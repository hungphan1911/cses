#!/usr/bin/env bash

# Compile one CSES solution, run its official tests, and record a first success.
#
# Usage:
#   ./run-tests.sh weird-algorithm
#   ./run-tests.sh introductory-problems/weird-algorithm

set -euo pipefail

if [[ $# -ne 1 ]]; then
    echo "Usage: $0 <problem-slug | category-slug/problem-slug>" >&2
    exit 2
fi

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
DATA_ROOT="${CSES_DATA_DIR:-$SCRIPT_DIR/cses-problemset}"
SOLUTIONS_ROOT="${SOLUTIONS_DIR:-$SCRIPT_DIR/solutions}"
README_PATH="${README_FILE:-$SCRIPT_DIR/README.md}"
BINARY_PATH="$SCRIPT_DIR/a"
COMPILER="${CXX:-g++}"
TIMEOUT_SECONDS="${TEST_TIMEOUT:-10}"
PROBLEM_KEY="${1%/}"

case "$PROBLEM_KEY" in
    ""|/*|*".."*)
        echo "Invalid problem path: $PROBLEM_KEY" >&2
        exit 2
        ;;
esac

if [[ ! -d "$DATA_ROOT" ]]; then
    echo "Problem-set directory not found: $DATA_ROOT" >&2
    echo "Run cses_scraper.py first or set CSES_DATA_DIR." >&2
    exit 2
fi

if [[ "$PROBLEM_KEY" == */* ]]; then
    PROBLEM_DIR="$DATA_ROOT/$PROBLEM_KEY"
    PROBLEM_SLUG="${PROBLEM_KEY##*/}"
    if [[ ! -d "$PROBLEM_DIR" ]]; then
        echo "Problem directory not found: $PROBLEM_DIR" >&2
        exit 2
    fi
else
    PROBLEM_SLUG="$PROBLEM_KEY"
    MATCHES=()
    for CATEGORY_DIR in "$DATA_ROOT"/*; do
        [[ -d "$CATEGORY_DIR" ]] || continue
        if [[ -d "$CATEGORY_DIR/$PROBLEM_SLUG" ]]; then
            MATCHES+=("$CATEGORY_DIR/$PROBLEM_SLUG")
        fi
    done
    if [[ ${#MATCHES[@]} -eq 0 ]]; then
        echo "No problem named '$PROBLEM_SLUG' was found under $DATA_ROOT." >&2
        exit 2
    fi
    if [[ ${#MATCHES[@]} -gt 1 ]]; then
        echo "Problem slug '$PROBLEM_SLUG' is ambiguous. Use category/problem:" >&2
        printf '  %s\n' "${MATCHES[@]#$DATA_ROOT/}" >&2
        exit 2
    fi
    PROBLEM_DIR="${MATCHES[0]}"
fi

SOLUTION_STEM="${PROBLEM_SLUG//-/}"
SOLUTION_PATH="$SOLUTIONS_ROOT/$SOLUTION_STEM.cpp"
TESTS_DIR="$PROBLEM_DIR/tests"

if [[ ! -f "$SOLUTION_PATH" ]]; then
    echo "Solution not found: $SOLUTION_PATH" >&2
    echo "Expected '$PROBLEM_SLUG' to map to '$SOLUTION_STEM.cpp'." >&2
    exit 2
fi

if [[ ! -d "$TESTS_DIR" ]]; then
    echo "Tests directory not found: $TESTS_DIR" >&2
    exit 2
fi

INPUTS=("$TESTS_DIR"/*.in)
if [[ ! -f "${INPUTS[0]}" ]]; then
    echo "No .in test files found in $TESTS_DIR" >&2
    exit 2
fi

for INPUT_PATH in "${INPUTS[@]}"; do
    EXPECTED_PATH="${INPUT_PATH%.in}.out"
    if [[ ! -f "$EXPECTED_PATH" ]]; then
        echo "Missing expected output for $(basename "$INPUT_PATH"): $EXPECTED_PATH" >&2
        exit 2
    fi
done

if ! command -v "$COMPILER" >/dev/null 2>&1; then
    echo "C++ compiler not found: $COMPILER" >&2
    echo "Set CXX to clang++ or another compiler if needed." >&2
    exit 2
fi

if ! command -v python3 >/dev/null 2>&1; then
    echo "python3 is required for portable timeouts, output comparison, and README updates." >&2
    exit 2
fi

DEFAULT_FLAGS="-std=c++20 -O2"
read -r -a COMPILE_FLAGS <<< "${CXXFLAGS:-$DEFAULT_FLAGS}"

echo "Compiling $SOLUTION_PATH"
echo "  $COMPILER ${COMPILE_FLAGS[*]} -o $BINARY_PATH"
"$COMPILER" "${COMPILE_FLAGS[@]}" "$SOLUTION_PATH" -o "$BINARY_PATH"

TEMP_DIR="$(mktemp -d "${TMPDIR:-/tmp}/cses-run.XXXXXX")"
cleanup() {
    if [[ -n "${TEMP_DIR:-}" && -d "$TEMP_DIR" ]]; then
        rm -rf -- "$TEMP_DIR"
    fi
}
trap cleanup EXIT
trap 'exit 130' INT
trap 'exit 143' TERM

PASSED=0
TOTAL=${#INPUTS[@]}

for INPUT_PATH in "${INPUTS[@]}"; do
    TEST_NAME="$(basename "$INPUT_PATH" .in)"
    EXPECTED_PATH="${INPUT_PATH%.in}.out"
    ACTUAL_PATH="$TEMP_DIR/$TEST_NAME.actual"
    STDERR_PATH="$TEMP_DIR/$TEST_NAME.stderr"

    if python3 - "$BINARY_PATH" "$INPUT_PATH" "$EXPECTED_PATH" \
        "$ACTUAL_PATH" "$STDERR_PATH" "$TIMEOUT_SECONDS" <<'PY'
import math
import subprocess
import sys
from pathlib import Path

binary, input_name, expected_name, actual_name, stderr_name, timeout_text = sys.argv[1:]
timeout = float(timeout_text)

try:
    with open(input_name, "rb") as source, open(actual_name, "wb") as output, open(stderr_name, "wb") as errors:
        result = subprocess.run(
            [binary],
            stdin=source,
            stdout=output,
            stderr=errors,
            timeout=timeout,
            check=False,
        )
except subprocess.TimeoutExpired:
    print(f"TIME LIMIT EXCEEDED (>{timeout:g}s)", file=sys.stderr)
    raise SystemExit(3)

if result.returncode != 0:
    diagnostic = Path(stderr_name).read_text(encoding="utf-8", errors="replace")[:2000]
    print(f"RUNTIME ERROR (exit code {result.returncode})", file=sys.stderr)
    if diagnostic:
        print(diagnostic, file=sys.stderr)
    raise SystemExit(2)

expected = Path(expected_name).read_text(encoding="utf-8", errors="replace").split()
actual = Path(actual_name).read_text(encoding="utf-8", errors="replace").split()

def tokens_equal(wanted: str, received: str) -> bool:
    if wanted == received:
        return True
    try:
        wanted_number = float(wanted)
        received_number = float(received)
    except ValueError:
        return False
    return math.isclose(wanted_number, received_number, rel_tol=1e-6, abs_tol=1e-9)

matches = len(expected) == len(actual) and all(
    tokens_equal(wanted, received) for wanted, received in zip(expected, actual)
)
if not matches:
    print("WRONG ANSWER", file=sys.stderr)
    print("Expected tokens:", " ".join(expected[:40]), file=sys.stderr)
    print("Actual tokens:  ", " ".join(actual[:40]), file=sys.stderr)
    if len(expected) > 40 or len(actual) > 40:
        print("(output truncated to 40 tokens)", file=sys.stderr)
    raise SystemExit(1)
PY
    then
        PASSED=$((PASSED + 1))
        printf '[%d/%d] PASS %s\n' "$PASSED" "$TOTAL" "$TEST_NAME"
    else
        STATUS=$?
        printf '[%d/%d] FAIL %s\n' "$((PASSED + 1))" "$TOTAL" "$TEST_NAME" >&2
        echo "README was not changed." >&2
        exit "$STATUS"
    fi
done

RELATIVE_PROBLEM_PATH="${PROBLEM_DIR#$DATA_ROOT/}"
MANIFEST_PATH="$DATA_ROOT/scrape-manifest.json"

python3 - "$README_PATH" "$MANIFEST_PATH" "$RELATIVE_PROBLEM_PATH" "$PROBLEM_SLUG" <<'PY'
import json
import os
import re
import sys
import tempfile
from datetime import date
from pathlib import Path

readme_path = Path(sys.argv[1])
manifest_path = Path(sys.argv[2])
relative_path = sys.argv[3]
problem_slug = sys.argv[4]

metadata = None
if manifest_path.is_file():
    try:
        manifest = json.loads(manifest_path.read_text(encoding="utf-8"))
        for record in manifest.get("problems", {}).values():
            if isinstance(record, dict) and record.get("path") == relative_path:
                metadata = record
                break
    except (OSError, ValueError, json.JSONDecodeError):
        pass

if metadata:
    task_id = str(metadata.get("id", relative_path))
    title = str(metadata.get("title", problem_slug.replace("-", " ").title()))
    category = str(metadata.get("category", relative_path.split("/", 1)[0]))
    url = str(metadata.get("url", ""))
    marker = f"<!-- cses-task:{task_id} -->"
else:
    title = problem_slug.replace("-", " ").title()
    category = relative_path.split("/", 1)[0].replace("-", " ").title()
    url = ""
    marker = f"<!-- cses-path:{relative_path} -->"

start_marker = "<!-- CSES_PROGRESS_START -->"
end_marker = "<!-- CSES_PROGRESS_END -->"
content = readme_path.read_text(encoding="utf-8") if readme_path.exists() else ""

if marker in content:
    print(f"Already recorded in {readme_path}: {title}")
    raise SystemExit(0)

def escape_cell(value: str) -> str:
    return value.replace("|", "\\|").replace("\n", " ")

problem_cell = f"[{escape_cell(title)}]({url})" if url else escape_cell(title)
new_row = (
    f"| {problem_cell} | {escape_cell(category)} | "
    f"{date.today().isoformat()} {marker} |"
)

if start_marker in content and end_marker in content:
    before, remainder = content.split(start_marker, 1)
    old_section, after = remainder.split(end_marker, 1)
    rows = [
        line for line in old_section.splitlines()
        if line.startswith("| ") and "<!-- cses-" in line
    ]
    rows.append(new_row)
else:
    before = content.rstrip() + ("\n\n" if content.strip() else "")
    after = ""
    rows = [new_row]

updated = date.today().isoformat()
section = "\n".join([
    start_marker,
    "## CSES Progress",
    "",
    f"**Completed problems:** {len(rows)}  ",
    f"**Last updated:** {updated}",
    "",
    "| Problem | Category | Completed |",
    "|---|---|---|",
    *rows,
    end_marker,
])
new_content = before + section + after
if not new_content.endswith("\n"):
    new_content += "\n"

readme_path.parent.mkdir(parents=True, exist_ok=True)
descriptor, temp_name = tempfile.mkstemp(prefix=readme_path.name + ".", dir=readme_path.parent)
try:
    with os.fdopen(descriptor, "w", encoding="utf-8") as output:
        output.write(new_content)
    os.replace(temp_name, readme_path)
except Exception:
    try:
        os.unlink(temp_name)
    except OSError:
        pass
    raise

print(f"Recorded completion in {readme_path}: {title}")
PY

echo "All $TOTAL tests passed for $PROBLEM_SLUG."

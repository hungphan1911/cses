#!/usr/bin/env python3

import argparse
import json
from pathlib import Path


def generate_markdown(data: dict) -> str:
    problems = data["problems"]

    lines = [
        "# CSES Problem Set",
        "",
    ]

    current_category = None

    for problem in problems.values():
        category = problem["category"]
        title = problem["title"]
        url = problem["url"]

        if category != current_category:
            if current_category is not None:
                lines.append("")

            lines.append(f"## {category}")
            lines.append("")
            current_category = category

        lines.append(f"- [{title}]({url})")

    lines.append("")
    return "\n".join(lines)


def main() -> None:
    parser = argparse.ArgumentParser(
        description="Convert a CSES problem catalog JSON file to Markdown."
    )
    parser.add_argument(
        "input",
        type=Path,
        help="Path to the input JSON file",
    )
    parser.add_argument(
        "output",
        type=Path,
        nargs="?",
        default=Path("PROBLEMS.md"),
        help="Path to the output Markdown file (default: PROBLEMS.md)",
    )

    args = parser.parse_args()

    with args.input.open("r", encoding="utf-8") as f:
        data = json.load(f)

    markdown = generate_markdown(data)

    args.output.write_text(markdown, encoding="utf-8")

    print(f"Wrote {len(data['problems'])} problems to {args.output}")


if __name__ == "__main__":
    main()
import { describe, expect, it } from 'vitest';
import { getLanguageForPath, renderCodeLines } from './syntax';

describe('syntax helpers', () => {
  it.each([
    ['main.cpp', 'cpp'],
    ['header.hpp', 'cpp'],
    ['script.py', 'python'],
    ['lib.rs', 'rust'],
    ['query.sql', 'sql'],
    ['README.md', 'markdown'],
    ['unknown.xyz', 'plaintext']
  ])('maps %s to %s', (path, language) => {
    expect(getLanguageForPath(path)).toBe(language);
  });

  it('renders line-numbered highlighted lines while preserving raw source separately', () => {
    const rendered = renderCodeLines('int main() {\n  return 0;\n}\n', 'main.cpp');

    expect(rendered.raw).toBe('int main() {\n  return 0;\n}\n');
    expect(rendered.lines).toHaveLength(4);
    expect(rendered.lines[0]?.number).toBe(1);
    expect(rendered.lines[1]?.html).toContain('return');
    expect(rendered.lines[3]?.html).toBe('');
  });

  it('keeps each rendered line as balanced markup', () => {
    const rendered = renderCodeLines('/* hello\nworld */\nint x;', 'main.cpp');

    for (const line of rendered.lines) {
      const openedSpans = line.html.match(/<span\b/g)?.length ?? 0;
      const closedSpans = line.html.match(/<\/span>/g)?.length ?? 0;
      expect(openedSpans).toBe(closedSpans);
    }
  });
});

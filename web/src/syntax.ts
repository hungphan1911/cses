import Prism from 'prismjs';
import 'prismjs/components/prism-bash';
import 'prismjs/components/prism-c';
import 'prismjs/components/prism-cpp';
import 'prismjs/components/prism-csharp';
import 'prismjs/components/prism-go';
import 'prismjs/components/prism-java';
import 'prismjs/components/prism-json';
import 'prismjs/components/prism-markdown';
import 'prismjs/components/prism-python';
import 'prismjs/components/prism-rust';
import 'prismjs/components/prism-sql';
import 'prismjs/components/prism-toml';
import 'prismjs/components/prism-typescript';
import 'prismjs/components/prism-yaml';

export interface RenderedLine {
  number: number;
  html: string;
}

export interface RenderedCode {
  raw: string;
  language: string;
  lines: RenderedLine[];
}

const extensionLanguage = new Map<string, string>([
  ['.cpp', 'cpp'],
  ['.cc', 'cpp'],
  ['.hpp', 'cpp'],
  ['.h', 'cpp'],
  ['.py', 'python'],
  ['.rs', 'rust'],
  ['.go', 'go'],
  ['.java', 'java'],
  ['.cs', 'csharp'],
  ['.js', 'javascript'],
  ['.ts', 'typescript'],
  ['.sh', 'bash'],
  ['.md', 'markdown'],
  ['.json', 'json'],
  ['.yaml', 'yaml'],
  ['.yml', 'yaml'],
  ['.toml', 'toml'],
  ['.sql', 'sql']
]);

export function getLanguageForPath(path: string): string {
  const extension = path.match(/\.[^.]+$/)?.[0].toLowerCase();
  return extension ? extensionLanguage.get(extension) ?? 'plaintext' : 'plaintext';
}

export function renderCodeLines(source: string, path: string): RenderedCode {
  const language = getLanguageForPath(path);
  const grammar = Prism.languages[language];

  return {
    raw: source,
    language,
    lines: source.split('\n').map((line, index) => ({
      number: index + 1,
      html: grammar ? Prism.highlight(line, grammar, language) : escapeHtml(line)
    }))
  };
}

function escapeHtml(value: string): string {
  return value
    .replace(/&/g, '&amp;')
    .replace(/</g, '&lt;')
    .replace(/>/g, '&gt;')
    .replace(/"/g, '&quot;')
    .replace(/'/g, '&#39;');
}

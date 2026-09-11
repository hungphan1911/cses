export interface TemplateFile {
  path: string;
  content: string;
}

export type RawTemplateModules = Record<string, string>;

export function buildTemplateManifest(modules: RawTemplateModules): TemplateFile[] {
  return Object.entries(modules)
    .map(([modulePath, content]) => ({
      path: normalizeTemplatePath(modulePath),
      content
    }))
    .sort((a, b) => a.path.localeCompare(b.path, undefined, { sensitivity: 'base' }));
}

export function filterTemplates(files: TemplateFile[], query: string): TemplateFile[] {
  const needle = query.trim().toLowerCase();
  if (!needle) {
    return files;
  }

  return files.filter((file) => file.path.toLowerCase().includes(needle));
}

export function getInitialTemplatePath(files: TemplateFile[], search: string, hash: string): string | null {
  if (files.length === 0) {
    return null;
  }

  const params = new URLSearchParams(search);
  const requested = params.get('file') ?? decodeHash(hash);

  if (requested && files.some((file) => file.path === requested)) {
    return requested;
  }

  return files[0]?.path ?? null;
}

function decodeHash(hash: string): string | null {
  const trimmed = hash.replace(/^#/, '').trim();
  return trimmed ? decodeURIComponent(trimmed) : null;
}

function normalizeTemplatePath(modulePath: string): string {
  return modulePath
    .replace(/\?raw$/, '')
    .replace(/^(\.\.\/)+templates?\//, '')
    .replace(/\\/g, '/');
}

import { describe, expect, it } from 'vitest';
import {
  buildTemplateManifest,
  filterTemplates,
  getInitialTemplatePath,
  type RawTemplateModules
} from './template-manifest';

describe('template manifest', () => {
  it('normalizes template roots and sorts relative paths alphabetically', () => {
    const modules: RawTemplateModules = {
      '../../templates/segmenttree.cpp?raw': 'segment tree',
      '../../template/graph/dijkstra.cpp?raw': 'dijkstra',
      '../../templates/fenwick.cpp?raw': 'fenwick'
    };

    expect(buildTemplateManifest(modules)).toEqual([
      { path: 'fenwick.cpp', content: 'fenwick' },
      { path: 'graph/dijkstra.cpp', content: 'dijkstra' },
      { path: 'segmenttree.cpp', content: 'segment tree' }
    ]);
  });

  it('filters paths with case-insensitive substring search', () => {
    const templates = buildTemplateManifest({
      '../../templates/graph/Dijkstra.cpp?raw': 'dijkstra',
      '../../templates/data/fenwick.cpp?raw': 'fenwick'
    });

    expect(filterTemplates(templates, 'dijk')).toEqual([{ path: 'graph/Dijkstra.cpp', content: 'dijkstra' }]);
    expect(filterTemplates(templates, 'DATA/')).toEqual([{ path: 'data/fenwick.cpp', content: 'fenwick' }]);
  });

  it('selects the URL requested template when present and falls back to the first template', () => {
    const templates = buildTemplateManifest({
      '../../templates/a.cpp?raw': 'a',
      '../../templates/b.cpp?raw': 'b'
    });

    expect(getInitialTemplatePath(templates, '?file=b.cpp', '')).toBe('b.cpp');
    expect(getInitialTemplatePath(templates, '', '#a.cpp')).toBe('a.cpp');
    expect(getInitialTemplatePath(templates, '?file=missing.cpp', '')).toBe('a.cpp');
  });
});

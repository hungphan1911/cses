import { buildTemplateManifest, type RawTemplateModules } from './template-manifest';

const templateModules = import.meta.glob('../../{template,templates}/**/*', {
  eager: true,
  query: '?raw',
  import: 'default'
}) as RawTemplateModules;

export const templates = buildTemplateManifest(templateModules);

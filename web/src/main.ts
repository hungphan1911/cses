import './styles.css';
import { filterTemplates, getInitialTemplatePath, type TemplateFile } from './template-manifest';
import { templates } from './templates';
import { renderCodeLines } from './syntax';

const app = document.querySelector<HTMLDivElement>('#app');

if (!app) {
  throw new Error('Missing #app root element');
}

let selectedPath = getInitialTemplatePath(templates, window.location.search, window.location.hash);
let searchQuery = '';
let copyResetTimer: number | undefined;

app.innerHTML = `
  <div class="shell">
    <header class="topbar">
      <div>
        <p class="eyebrow">Template notebook</p>
        <h1>Templates</h1>
      </div>
      <label class="search">
        <span class="sr-only">Search templates</span>
        <input id="search-input" type="search" placeholder="Search files" autocomplete="off" />
      </label>
    </header>

    <main class="workspace">
      <aside class="sidebar" aria-label="Template files">
        <div class="sidebar-title">
          <span>Saved snippets</span>
          <span id="file-count" class="file-count"></span>
        </div>
        <nav id="file-list" class="file-list"></nav>
      </aside>

      <section class="viewer" aria-live="polite">
        <div class="viewer-header">
          <div>
            <p class="viewer-kicker">Selected template</p>
            <h2 id="selected-name"></h2>
          </div>
          <button id="copy-button" class="copy-button" type="button">Copy</button>
        </div>
        <div id="code-wrap" class="code-wrap"></div>
      </section>
    </main>
  </div>
`;

const searchInput = queryRequired<HTMLInputElement>('#search-input');
const fileList = queryRequired<HTMLElement>('#file-list');
const fileCount = queryRequired<HTMLElement>('#file-count');
const selectedName = queryRequired<HTMLElement>('#selected-name');
const copyButton = queryRequired<HTMLButtonElement>('#copy-button');
const codeWrap = queryRequired<HTMLElement>('#code-wrap');

searchInput.addEventListener('input', () => {
  searchQuery = searchInput.value;
  const filtered = filterTemplates(templates, searchQuery);
  if (selectedPath && !filtered.some((file) => file.path === selectedPath)) {
    selectedPath = filtered[0]?.path ?? null;
  }
  render();
});

copyButton.addEventListener('click', async () => {
  const selected = getSelectedTemplate();
  if (!selected) {
    return;
  }

  await copyText(selected.content);
  copyButton.textContent = 'Copied!';
  window.clearTimeout(copyResetTimer);
  copyResetTimer = window.setTimeout(() => {
    copyButton.textContent = 'Copy';
  }, 1400);
});

document.addEventListener('keydown', (event) => {
  if ((event.metaKey || event.ctrlKey) && event.key.toLowerCase() === 'k') {
    event.preventDefault();
    searchInput.focus();
    searchInput.select();
  }
});

window.addEventListener('popstate', () => {
  selectedPath = getInitialTemplatePath(templates, window.location.search, window.location.hash);
  render();
});

render();

function render(): void {
  const filtered = filterTemplates(templates, searchQuery);
  const selected = getSelectedTemplate();

  fileCount.textContent = `${filtered.length}/${templates.length}`;
  renderFileList(filtered);

  if (!selected) {
    selectedName.textContent = 'No templates found';
    copyButton.disabled = true;
    codeWrap.innerHTML = '<div class="empty-state">Add files under templates/ and rebuild the site.</div>';
    return;
  }

  copyButton.disabled = false;
  selectedName.textContent = selected.path;
  renderCode(selected);
}

function renderFileList(files: TemplateFile[]): void {
  if (files.length === 0) {
    fileList.innerHTML = '<p class="empty-list">No matching files</p>';
    return;
  }

  fileList.replaceChildren(
    ...files.map((file) => {
      const button = document.createElement('button');
      button.type = 'button';
      button.className = file.path === selectedPath ? 'file-item is-active' : 'file-item';
      button.textContent = file.path;
      button.setAttribute('aria-current', file.path === selectedPath ? 'page' : 'false');
      button.addEventListener('click', () => selectTemplate(file.path));
      return button;
    })
  );
}

function renderCode(file: TemplateFile): void {
  const rendered = renderCodeLines(file.content, file.path);
  const code = document.createElement('code');
  code.dataset.language = rendered.language;

  const rows = rendered.lines.map((line) => {
    const lineElement = document.createElement('div');
    lineElement.className = 'code-line';
    lineElement.innerHTML = `<span class="line-number">${line.number}</span><span class="line-source">${line.html || ' '}</span>`;
    return lineElement;
  });

  const pre = document.createElement('pre');
  pre.className = 'code-block';
  code.replaceChildren(...rows);
  pre.append(code);
  codeWrap.replaceChildren(pre);
}

function selectTemplate(path: string): void {
  selectedPath = path;
  const url = new URL(window.location.href);
  url.searchParams.set('file', path);
  url.hash = '';
  window.history.pushState({}, '', url);
  render();
}

function getSelectedTemplate(): TemplateFile | null {
  return templates.find((file) => file.path === selectedPath) ?? null;
}

async function copyText(value: string): Promise<void> {
  if (navigator.clipboard?.writeText) {
    await navigator.clipboard.writeText(value);
    return;
  }

  const textarea = document.createElement('textarea');
  textarea.value = value;
  textarea.setAttribute('readonly', '');
  textarea.className = 'clipboard-fallback';
  document.body.append(textarea);
  textarea.select();
  document.execCommand('copy');
  textarea.remove();
}

function queryRequired<T extends Element>(selector: string): T {
  const element = document.querySelector<T>(selector);
  if (!element) {
    throw new Error(`Missing required element: ${selector}`);
  }

  return element;
}

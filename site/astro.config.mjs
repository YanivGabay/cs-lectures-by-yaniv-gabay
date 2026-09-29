// @ts-check
import { defineConfig } from 'astro/config';
import tailwindcss from '@tailwindcss/vite';

export default defineConfig({
  site: 'https://cs-lectures.pages.dev',
  output: 'static',
  vite: {
    plugins: [tailwindcss()],
    define: {
      'import.meta.env.PUBLIC_TERMINAL_URL': JSON.stringify(process.env.PUBLIC_TERMINAL_URL || ''),
    },
  },
  markdown: {
    shikiConfig: {
      themes: {
        light: 'github-light',
        dark: 'github-dark',
      },
    },
  },
});

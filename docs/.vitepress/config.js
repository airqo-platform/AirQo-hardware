import { defineConfig } from 'vitepress'

export default defineConfig({
  title: 'AirQo Hardware',
  description: 'Documentation for the AirQo Binos Air Quality Monitor and BAM Data Logger',
  base: '/AirQo-hardware/',

  head: [
    ['link', { rel: 'icon', href: '/favicon.ico' }],
    ['link', { rel: 'preconnect', href: 'https://fonts.googleapis.com' }],
  ],

  themeConfig: {
    logo: '/airqo-logo.png',
    siteTitle: 'Hardware Docs',

    search: {
      provider: 'local',
    },

    nav: [
      { text: 'Home', link: '/' },
      {
        text: 'Device',
        items: [
          { text: 'Overview', link: '/device/overview' },
          { text: 'Technical Specification', link: '/device/technical-spec' },
          { text: 'Hardware Evolution', link: '/device/evolution' },
        ],
      },
      {
        text: 'Firmware',
        items: [
          { text: 'Overview', link: '/firmware/overview' },
          { text: 'SD Card Organisation', link: '/firmware/sd-card' },
          { text: 'Configuration & Flashing', link: '/firmware/configuration' },
        ],
      },
      { text: 'Deployment', link: '/deployment' },
      {
        text: 'Maintenance',
        items: [
          { text: 'Overview', link: '/maintenance/' },
          { text: 'Toolkit', link: '/maintenance/toolkit' },
          { text: 'Device Diagnosis', link: '/maintenance/diagnosis' },
          { text: 'System Maintenance', link: '/maintenance/system' },
        ],
      },
      {
        text: 'Data Logger',
        items: [
          { text: 'Overview', link: '/data-logger/' },
          { text: 'Technical Specification', link: '/data-logger/technical-spec' },
          { text: 'Installation', link: '/data-logger/installation' },
          { text: 'Data Access & Debugging', link: '/data-logger/data-access' },
        ],
      },
      { text: 'FAQs', link: '/faqs' },
    ],

    sidebar: {
      '/device/': [
        {
          text: 'Device',
          items: [
            { text: 'Overview', link: '/device/overview' },
            { text: 'Technical Specification', link: '/device/technical-spec' },
            { text: 'Hardware Evolution', link: '/device/evolution' },
          ],
        },
      ],
      '/firmware/': [
        {
          text: 'Firmware',
          items: [
            { text: 'Overview', link: '/firmware/overview' },
            { text: 'SD Card Organisation', link: '/firmware/sd-card' },
            { text: 'Configuration & Flashing', link: '/firmware/configuration' },
          ],
        },
      ],
      '/maintenance/': [
        {
          text: 'Maintenance',
          items: [
            { text: 'Overview', link: '/maintenance/' },
            { text: 'Toolkit', link: '/maintenance/toolkit' },
            { text: 'Device Diagnosis', link: '/maintenance/diagnosis' },
            { text: 'System Maintenance', link: '/maintenance/system' },
          ],
        },
      ],
      '/data-logger/': [
        {
          text: 'BAM Data Logger',
          items: [
            { text: 'Overview', link: '/data-logger/' },
            { text: 'Technical Specification', link: '/data-logger/technical-spec' },
            { text: 'Installation', link: '/data-logger/installation' },
            { text: 'Data Access & Debugging', link: '/data-logger/data-access' },
          ],
        },
      ],
    },

    editLink: {
      pattern: 'https://github.com/airqo-platform/AirQo-hardware/edit/main/docs/:path',
      text: 'Edit this page on GitHub',
    },

    footer: {
      message: 'AirQo Hardware Documentation',
      copyright: '© 2024 AirQo Platform. All rights reserved.',
    },

    socialLinks: [
      { icon: 'github', link: 'https://github.com/airqo-platform/AirQo-hardware' },
    ],
  },
})

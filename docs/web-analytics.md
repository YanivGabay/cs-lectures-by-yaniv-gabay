# Turning on Cloudflare Web Analytics (page views)

The status page's **Usage** section counts *terminals opened* (Run, Run pair, full terminal).
It cannot see people who only read the pages. Cloudflare Web Analytics adds that: visits per page,
per day, countries, desktop vs. phone. It is free, sets **no cookies** and collects no personal data,
so no consent banner is needed.

This is a dashboard switch — the Wrangler login used for deploys cannot turn it on.

## Steps (about 2 minutes)

1. Sign in at <https://dash.cloudflare.com> with the account that owns the site (yaniv242@gmail.com).
2. Left menu: **Workers & Pages** → click the **cs-lectures** project (the Pages site, not
   `cs-lectures-terminal`).
3. Open the **Metrics** tab → in the **Web Analytics** box, click **Enable**.
   Cloudflare adds its small analytics script to the site automatically.
4. The script is added on the **next deployment**. Either wait for the next change to be deployed, or
   ask Claude to redeploy the site (nothing in the code changes).
5. After that, numbers appear under **Workers & Pages → cs-lectures → Metrics**, and in
   **Analytics & Logs → Web Analytics**. Expect the first data within a few minutes of visits.

If the dashboard looks different, search the Cloudflare docs for "Pages Web Analytics" — the switch
sometimes moves between menus.

## How to check it is on

After the redeploy, the site's HTML loads `static.cloudflareinsights.com/beacon.min.js`:

```bash
curl -s https://cs-lectures.pages.dev/ | grep -o 'cloudflareinsights[^"]*' | head -1
```

Prints a line → on. Prints nothing → not yet (check step 3, then redeploy).

## What you will and will not see

| You see | You do not see |
|---|---|
| Page views and visits per page, per day | Names, emails, IP addresses |
| Which lessons are read most | Individual people or their history |
| Countries, browsers, desktop vs. phone | Anything typed in the terminals |

Turning it off later: the same **Metrics → Web Analytics** box has a **Disable** button.

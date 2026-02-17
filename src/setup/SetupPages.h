#pragma once
#include <Arduino.h>

static const char SETUP_INDEX_HTML[] PROGMEM = R"HTML(
<!doctype html>
<html>
<head>
  <meta charset="utf-8"/>
  <meta name="viewport" content="width=device-width, initial-scale=1"/>
  <title>University Assignment Tracker Setup</title>
  <style>
    body { font-family: Arial, sans-serif; margin: 16px; }
    input { width: 100%; padding: 8px; margin: 6px 0 12px 0; box-sizing: border-box; }
    .row { display: grid; grid-template-columns: 1fr 2fr; gap: 12px; }
    .card { border: 1px solid #ddd; padding: 12px; border-radius: 8px; margin-bottom: 16px; }
    button { padding: 10px 14px; font-size: 16px; }
    code { background: #f4f4f4; padding: 2px 4px; border-radius: 4px; }
  </style>
</head>
<body>
  <h2>University Assignment Tracker Setup</h2>

  <div class="card">
    <p>Fill WiFi and course calendar feed URLs. Click <b>Save</b> to persist into NVS and reboot.</p>
    <p>After reboot, the device will run in normal mode.</p>
  </div>

  <form method="POST" action="/save">
    <div class="card">
      <h3>WiFi</h3>
      <label>SSID</label>
      <input name="ssid" placeholder="WiFi SSID" required>

      <label>Password</label>
      <input name="pass" type="password" placeholder="WiFi Password" required>

      <label>Timezone Offset (seconds)</label>
      <input name="tz" placeholder="-25200 for MST (UTC-7)" value="-25200">
    </div>

    <div class="card">
      <h3>Courses (up to 8)</h3>

      <div class="row">
        <div><label>Course 1 Name</label><input name="c0n" placeholder="Course 1"></div>
        <div><label>Course 1 URL</label><input name="c0u" placeholder="https://...feed.ics"></div>
      </div>

      <div class="row">
        <div><label>Course 2 Name</label><input name="c1n" placeholder="Course 2"></div>
        <div><label>Course 2 URL</label><input name="c1u" placeholder="https://...feed.ics"></div>
      </div>

      <div class="row">
        <div><label>Course 3 Name</label><input name="c2n" placeholder="Course 3"></div>
        <div><label>Course 3 URL</label><input name="c2u" placeholder="https://...feed.ics"></div>
      </div>

      <div class="row">
        <div><label>Course 4 Name</label><input name="c3n" placeholder="Course 4"></div>
        <div><label>Course 4 URL</label><input name="c3u" placeholder="https://...feed.ics"></div>
      </div>

      <div class="row">
        <div><label>Course 5 Name</label><input name="c4n" placeholder="(optional)"></div>
        <div><label>Course 5 URL</label><input name="c4u" placeholder="(optional)"></div>
      </div>

      <div class="row">
        <div><label>Course 6 Name</label><input name="c5n" placeholder="(optional)"></div>
        <div><label>Course 6 URL</label><input name="c5u" placeholder="(optional)"></div>
      </div>

      <div class="row">
        <div><label>Course 7 Name</label><input name="c6n" placeholder="(optional)"></div>
        <div><label>Course 7 URL</label><input name="c6u" placeholder="(optional)"></div>
      </div>

      <div class="row">
        <div><label>Course 8 Name</label><input name="c7n" placeholder="(optional)"></div>
        <div><label>Course 8 URL</label><input name="c7u" placeholder="(optional)"></div>
      </div>
    </div>

    <button type="submit">Save & Reboot</button>
  </form>

  <div class="card">
    <h3>Maintenance</h3>
    <form method="POST" action="/clear" onsubmit="return confirm('Clear saved config and reboot?');">
      <button type="submit">Clear Saved Config</button>
    </form>
  </div>

</body>
</html>
)HTML";
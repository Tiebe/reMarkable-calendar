# reMarkable Calendar

Calendar app for the reMarkable Paper Pro. Still very much in beta, see the todo below for what's left.

## Installation
1. Download the latest release [here](https://github.com/Tiebe/remarkable-calendar/releases).
2. Unzip the zip file in your `qt-resource-rebuilder` folder in `exthome`.
3. Copy the `libcalendar.so` file to your `extensions.d` folder.
4. Create a new folder called `calendar` in your `exthome` folder.
5. Create a new file here called `calendars.json`. This is where you will define your calendars. This extensions expects the following contents:
```
[
    {
        "name": "Calendar name",
        "icsLink": "https://example.org/calendar.ics"
    }
]
```

You can add multiple calendars here, simply by adding a new item in the top-level array. The `name` field is used for caching the calendar. These cached calendars will be saved in the `exthome/calendar` folder.

The `icsLink` expects a URL to a valid ICS file. You can get these from most calendar providers. See [here](https://support.google.com/calendar/answer/37648?hl=en#zippy=%2Cget-your-calendar-view-only%2Cpublic-address%2Csecret-address) for the instructions for Google Calendar. Make sure to use the secret address if the calendar is not public.

## TODO
- [X] Add month view
- [X] Add week view
- [ ] Add day view
- [ ] Add support for overlapping events
- [ ] Add support for recurring events
- [ ] Add detailed view
- [ ] Add color field to json, and use this to give different calendars different colors

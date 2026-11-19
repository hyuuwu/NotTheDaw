// QoL round 191 (upstream issues #181, #195): Ctrl+, opens Settings; Home / End jump to the start / end of the song

TEST (qol191_home_end_and_settings_keys)
{
    using namespace lynx::keys;
    KeyMap map;
    CHECK (map.conflicts().empty());
    for (const auto focus : { Context::global, Context::playlist, Context::pianoRoll, Context::channelRack, Context::mixer, Context::browser })
    {
        CHECK (map.find ({ home }, focus, false) == "transport.goToStart");
        CHECK (map.find ({ end }, focus, false) == "transport.goToEnd");
        CHECK (map.find ({ ',', ctrl }, focus, false) == "view.settings");
    }
    // the typing piano doesn't take Home / End (not printable), nor a Ctrl chord
    CHECK (map.find ({ home }, Context::pianoRoll, true) == "transport.goToStart");
    CHECK (map.find ({ ',', ctrl }, Context::pianoRoll, true) == "view.settings");
    // the Playlist's own Ctrl+Alt+Home (left resizing) stays its own
    CHECK (map.find ({ home, (std::uint8_t) (ctrl | alt) }, Context::playlist, false) == "playlist.leftResizing");
    // the chord's text round-trips (the shortcut editor and keys.json)
    CHECK (toString ({ ',', ctrl }) == "Ctrl+,");
    const auto parsed = parseChord ("Ctrl+,");
    CHECK (parsed.has_value() && parsed->key == ',' && parsed->mods == ctrl);
}

// FL Studio's official shortcut list (Image-Line's manual, "Keyboard shortcuts"), the keys this round adds
TEST (qol191_fl_shortcut_list)
{
    using namespace lynx::keys;
    KeyMap map;
    CHECK (map.conflicts().empty());
    const auto mods = [] (int m) { return (std::uint8_t) m; };
    // File: Ctrl+N saves a new version; Alt+1..9, 0 open the recent files
    CHECK (map.find ({ 'N', ctrl }, Context::global, false) == "file.saveNewVersion");
    CHECK (map.find ({ 'S', mods (ctrl | alt) }, Context::global, false) == "file.saveNewVersion");
    CHECK (map.find ({ '1', alt }, Context::playlist, false) == "file.recent1");
    CHECK (map.find ({ '0', alt }, Context::mixer, false) == "file.recent10");
    // Transport: numpad * and / step a bar; Alt+* / Alt+/ stay the marker jumps
    CHECK (map.find ({ numpadMultiply }, Context::global, false) == "transport.nextBar");
    CHECK (map.find ({ numpadDivide }, Context::global, false) == "transport.previousBar");
    CHECK (map.find ({ numpadMultiply, alt }, Context::global, false) == "transport.nextMarker");
    CHECK (map.find ({ numpad4, alt }, Context::global, false) == "typing.octave4");
    CHECK (map.find ({ 'H', mods (ctrl | shift) }, Context::global, false) == "view.arrangeWindows");
    // Channel Rack: Page Up / Down = filter groups, Ctrl+Enter = Playlist
    CHECK (map.find ({ pageUp }, Context::channelRack, false) == "rack.nextGroup");
    CHECK (map.find ({ pageDown }, Context::channelRack, false) == "rack.previousGroup");
    CHECK (map.find ({ enter, ctrl }, Context::channelRack, false) == "rack.focusPlaylist");
    // Piano roll: O, F, Shift+F
    CHECK (map.find ({ 'O' }, Context::pianoRoll, false) == "pianoRoll.porta");
    CHECK (map.find ({ 'F' }, Context::pianoRoll, false) == "pianoRoll.laneNext");
    CHECK (map.find ({ 'F', shift }, Context::pianoRoll, false) == "pianoRoll.lanePrevious");
    // Playlist: Insert, 5, 6, Shift+Q
    CHECK (map.find ({ insert }, Context::playlist, false) == "playlist.sliceAtPlayhead");
    CHECK (map.find ({ '5' }, Context::playlist, false) == "playlist.zoomSelection");
    CHECK (map.find ({ '6' }, Context::playlist, false) == "playlist.zoomPerformance");
    CHECK (map.find ({ 'Q', shift }, Context::playlist, false) == "playlist.quantizeStarts");
    // Mixer: Ctrl+A
    CHECK (map.find ({ 'A', ctrl }, Context::mixer, false) == "mixer.selectAll");
}

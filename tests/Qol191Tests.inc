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

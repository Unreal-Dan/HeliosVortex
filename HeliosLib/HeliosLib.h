
// Wrapper class around Helios as a whole, put high level apis that are used on
// desktop or in wasm but not in the core of Helios in this layer. Technically
// HeliosCLI would wrap this library to produce a CLI tool, but we already
// wrapped the Helios core so we should abstract some of that logic to here and
// simplify the CLI tool by just using this library directly
class HeliosLib
{
public:
    static bool init();
    static void cleanup();

    static void tick();
};


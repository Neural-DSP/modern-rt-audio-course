// Managing names with namespaces in C++

// Used for handling preset load errors
#include <optional>

// Used for storing "preset" values
#include <unordered_map>

// Included for debug logging
#include <iostream>

// Math functions
#include <string>

struct Preset
{
    Preset(const std::string& _name) : name { _name } {}

    // Dummy function to emulate preset save
    void save_parameter(const std::string& element, const std::string& property, float value)
    {
        std::cout << "SAVE -> elem: " << element << "; prop: " << property << "; value: " << value << std::endl;
        state[element + "_" + property] = value;
    }

    // Dummy function to emulate preset load
    std::optional<float> load_parameter(const std::string& element, const std::string& property) const
    {
        // check if parameter has been saved
        if (state.count(element + "_" + property))
        {
            float value = state.at(element + "_" + property);
            std::cout << "LOAD -> elem: " << element << "; prop: " << property << "; value: " << value << std::endl;
            return value;
        }

        std::cout << "FAILED LOAD -> elem: " << element << "; prop: " << property << ";" << std::endl;
        return {};
    }

    void print() const
    {
        std::cout << "Preset name: " << name << std::endl;
        for (auto& param : state)
            std::cout << "  " << param.first << ": " << param.second << std::endl;
    }

private:
    std::string name;
    std::unordered_map<std::string, float> state;
};

// Preset XML identifiers
namespace state
{
    namespace global
    {
        std::string type { "global" };

        std::string gain { "gain" };
        std::string blend { "blend" };
    }

    namespace distortion
    {
        // We can use the same variable name multiple times
        // as long as they are under different namespaces
        std::string type { "distortion" };

        std::string gain { "gain" };
        std::string tone { "tone" };
        std::string level { "level" };
    }

    namespace filter
    {
        std::string type { "filter" };

        std::string frequency { "frequency" };
        std::string resonance { "resonance" };
        std::string level { "level" };
    }
}

struct Plugin
{
    // prepare and process callbacks...

    // Save current plugin state to preset structure
    void save_parameters_to_preset(Preset& p)
    {
        {
            // The "using" keyword gives us access on this scope
            // to all the variables and methods under the namespace.
            using namespace state::global;
            p.save_parameter(type, gain, global_gain);
            p.save_parameter(type, blend, global_blend);
        }
        {
            using namespace state::distortion;
            p.save_parameter(type, gain, dist_gain);
            p.save_parameter(type, tone, dist_tone);
            p.save_parameter(type, level, dist_level);
        }
        {
            using namespace state::filter;
            p.save_parameter(type, frequency, filt_freq);
            p.save_parameter(type, resonance, filt_reso);
            p.save_parameter(type, level, filt_level);
        }
    }

    // Load parameters from preset structure to plugin internal state
    void load_parameters_from_preset(const Preset& p)
    {
        {
            using namespace state::global;
            if (auto param = p.load_parameter(type, gain)) global_gain = *param;
            if (auto param = p.load_parameter(type, blend)) global_blend = *param;
        }
        {
            using namespace state::distortion;
            if (auto param = p.load_parameter(type, gain)) dist_gain = *param;
            if (auto param = p.load_parameter(type, tone)) dist_tone = *param;
            if (auto param = p.load_parameter(type, level)) dist_level = *param;
        }
        {
            using namespace state::filter;
            if (auto param = p.load_parameter(type, frequency)) filt_freq = *param;
            if (auto param = p.load_parameter(type, resonance)) filt_reso = *param;
            if (auto param = p.load_parameter(type, level)) filt_level = *param;
        }
    }

    // Setters

    void set_global_gain(float val) { global_gain = val; }
    void set_global_blend(float val) { global_blend = val; }
    void set_dist_gain(float val) { dist_gain = val; }
    void set_dist_tone(float val) { dist_tone = val; }
    void set_dist_level(float val) { dist_level = val; }
    void set_filt_freq(float val) { filt_freq = val; }
    void set_filt_reso(float val) { filt_reso = val; }
    void set_filt_level(float val) { filt_level = val; }


    // Getters

    float get_global_gain() const { return global_gain; }
    float get_global_blend() const { return global_blend; }
    float get_dist_gain() const { return dist_gain; }
    float get_dist_tone() const { return dist_tone; }
    float get_dist_level() const { return dist_level; }
    float get_filt_freq() const { return filt_freq; }
    float get_filt_reso() const { return filt_reso; }
    float get_filt_level() const { return filt_level; }

private:
    float global_gain { 0.f };
    float global_blend { 0.5f };

    float dist_gain { 0.f };
    float dist_tone { 0.f };
    float dist_level { 0.f };

    float filt_freq { 1000.f };
    float filt_reso { 0.7071f };
    float filt_level { 0.f };
};

int main()
{
    // Let's create a some presets
    Preset default_preset("Default");
    Preset awesome_preset("Awesome");

    {
        // Plugin instance
        // Initial internal state with defualt values.
        Plugin plugin;

        // Save it to the default preset
        std::cout << std::endl; // Extra line break for readability
        plugin.save_parameters_to_preset(default_preset);

        // Print the default preset to verify
        std::cout << std::endl; // Extra line break for readability
        default_preset.print();

        // Now let's modify the plugin state
        plugin.set_global_gain(4.7f);
        plugin.set_global_blend(0.35f);
        plugin.set_dist_gain(-3.7);
        plugin.set_dist_tone(0.8f);
        plugin.set_dist_level(-2.6f);
        plugin.set_filt_freq(800.f);
        plugin.set_filt_reso(2.5f);
        plugin.set_filt_level(2.f);

        // And save it to a different preset
        std::cout << std::endl; // Extra line break for readability
        plugin.save_parameters_to_preset(awesome_preset);
    }

    {
        // Another plugin instance
        Plugin plugin;

        // Let's load a preset
        std::cout << std::endl; // Extra line break for readability
        plugin.load_parameters_from_preset(awesome_preset);
    }

    return 0;
}

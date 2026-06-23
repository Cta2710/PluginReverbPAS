cmake_minimum_required(VERSION 3.22)
project(MiPluginDELAY VERSION 0.1)

# Ajustá esta ruta a donde tengas JUCE clonado
add_subdirectory(C:/dev/JUCE ./JUCE)

juce_add_plugin(MiPluginDELAY
    FORMATS VST3
    PLUGIN_NAME "EarlyRef"
    PLUGIN_MANUFACTURER_CODE Cbas
    PLUGIN_CODE DelB
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    VST3_CATEGORIES "Fx"
    IS_MIDI_EFFECT FALSE
    COPY_PLUGIN_AFTER_BUILD FALSE
    VST3_CAN_REPLACE_VST2 FALSE
)

juce_generate_juce_header(MiPluginDELAY)

target_sources(MiPluginDELAY PRIVATE
    Source/Infr/PluginProcessor.cpp
    Source/Infr/PluginEditor.cpp
    Source/DSP/MultiTapDelay/MultiTapDelay.cpp
    Source/GUI/TableEditor.cpp
    Source/GUI/GraphEditor.cpp
    Source/GUI/SliderNumTaps.cpp

)

target_link_libraries(MiPluginDELAY PRIVATE
    juce::juce_audio_processors
    juce::juce_audio_utils
    juce::juce_dsp
)

target_compile_definitions(MiPluginDELAY PUBLIC
    JUCE_IGNORE_VST3_MISMATCHED_PARAMETER_ID_WARNING=1
)

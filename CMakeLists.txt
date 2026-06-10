# ============================================================
# CMakeLists.txt — MiTemplatePlugin
# Programación Aplicada al Sonido II — UNA
# ============================================================
#
# Este archivo le dice al compilador qué construir.
# No necesitan entenderlo en detalle — solo cambiar el nombre
# del plugin y la ruta de JUCE si es diferente.
#
# Compilar:
#   cd ~/Desktop/dev/UNA-PAS2/modulo-02-dft-fft/MiTemplatePlugin
#   rm -rf build && mkdir build && cd build && cmake .. && make
#
# ============================================================

cmake_minimum_required(VERSION 3.22)
project(MiPluginDELAY VERSION 0.1)

# Ajustá esta ruta a donde tengas JUCE clonado
add_subdirectory(C:/dev/JUCE ./JUCE)

juce_add_plugin(MiPluginDELAY
    FORMATS VST3
    PLUGIN_NAME "Delay3"
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
    Source/PluginProcessor.cpp
    Source/PluginEditor.cpp
)

target_link_libraries(MiPluginDELAY PRIVATE
    juce::juce_audio_processors
    juce::juce_audio_utils
    juce::juce_dsp
)

target_compile_definitions(MiPluginDELAY PUBLIC
    JUCE_IGNORE_VST3_MISMATCHED_PARAMETER_ID_WARNING=1
)

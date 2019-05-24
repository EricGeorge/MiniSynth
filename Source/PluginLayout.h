/*
  ==============================================================================

    PluginLayout.h
    Created: 23 May 2019 7:29:42pm
    Author:  Eric

  ==============================================================================
*/

#pragma once

const int pluginEditorWidth = 800;
const int pluginEditorHeight = 600;

const int controlBarX = 0;
const int controlBarY = 0;
const int controlBarWidth = pluginEditorWidth;
const int controlBarHeight = 50;

const int bottomBarHeight = 150;
const int bottomBarWidth = pluginEditorWidth;
const int bottomBarX = 0;
const int bottomBarY = pluginEditorHeight - bottomBarHeight;

const int centerPanelX = 0;
const int centerPanelY = controlBarHeight;
const int centerPanelWidth = pluginEditorWidth;
const int centerPanelHeight = pluginEditorHeight - controlBarHeight - bottomBarHeight;

const int midiKeyboardX = 0;
const int midiKeyboardY = pluginEditorHeight - bottomBarHeight;
const int midiKeyboardWidth = bottomBarWidth * 3 / 4;
const int midiKeyboardHeight = bottomBarHeight;
const int midiKeyboardKeyWidth = 32;

const int scopeX = midiKeyboardWidth;
const int scopeY = bottomBarY;
const int scopeWidth = pluginEditorWidth - midiKeyboardWidth;
const int scopeHeight = bottomBarHeight;

const int outputX = centerPanelWidth - centerPanelWidth / 8;
const int outputY = centerPanelY;
const int outputWidth = centerPanelWidth / 8;
const int outputHeight = centerPanelHeight;

const int oscillatorX = centerPanelX;
const int oscillatorY = centerPanelY;
const int oscillatorWidth = centerPanelWidth - outputWidth;
const int oscillatorHeight = centerPanelHeight;


/*******************************************************************************
 BEGIN_JUCE_MODULE_DECLARATION

  ID:                 mrta_utils
  vendor:             Neural DSP
  version:            0.0.1
  name:               Modern Real-Time Audio Utilities
  description:        Some utilities for easy plugin development. Developed for the Modern Real-Time Audio Programming Course.
  website:            http://www.neuraldsp.com
  license:            GPL
  minimumCppStandard: 17

  dependencies:       juce_audio_processors

 END_JUCE_MODULE_DECLARATION

*******************************************************************************/

#pragma once

#define MRTA_UTILS_H_INCLUDED

#include <juce_audio_processors/juce_audio_processors.h>

#include "Source/Parameter/ParameterFIFO.h"
#include "Source/Parameter/ParameterInfo.h"
#include "Source/Parameter/ParameterManager.h"
#include "Source/GUI/ParameterComponents.h"
#include "Source/GUI/GenericParameterEditor.h"


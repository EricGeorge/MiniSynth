/*
  ==============================================================================

    WaveViewPanel.cpp
    Created: 18 Jun 2019 7:40:40pm
    Author:  Eric

  ==============================================================================
*/

#include "WaveViewPanel.h"

#include "PluginCommonStyling.h"

WaveViewPanel::WaveViewPanel(SynthSound& sound)
:   sound(sound),
    frameIndex1(0),
    frameIndex2(0),
    interpFrac(0.0)
{

}

WaveViewPanel::~WaveViewPanel()
{
    
}

void WaveViewPanel::paint(Graphics& g)
{
    Component::paint(g);
    
    Rectangle<int> border(getLocalBounds().reduced(panelOutlineInset));

    // paint the background
    g.setColour(getCommonColours().panelBackground);
    g.fillAll();
    
    // paint the center line
    auto center = border.getHeight() / 2;
    g.setColour(getCommonColours().panelDetails);
    g.drawLine(Line<float>(border.getX(), center, border.getRight(), center));
    
    // paint the waveform
    Rectangle<int> inset = border;
    inset.reduce(2, 20);
    g.setColour(getCommonColours().detailContrast);

    if (waveform1 && waveform2)
    {
        auto numSamples = waveform1->getNumSamples();
        for (int i = 1; i < numSamples; ++i)
        {
            float x1 = jmap<float>(i - 1, 0, numSamples - 1, inset.getX(), inset.getRight());
            float y1 = jmap<float>(waveform1->getSample(i - 1), -1, 1, inset.getBottom(), inset.getY());

            float x2 = jmap<float>(i, 0, numSamples - 1, inset.getX(), inset.getRight());
            float y2 = jmap<float>(waveform1->getSample(i), -1, 1, inset.getBottom(), inset.getY());

            if (frameIndex2 != frameIndex1)
            {
                float y1Interp = jmap<float>(waveform2->getSample(i - 1), -1, 1, inset.getBottom(), inset.getY());
                float y2Interp = jmap<float>(waveform2->getSample(i), -1, 1, inset.getBottom(), inset.getY());

                y1 = linear_interp(y1, y1Interp, interpFrac);
                y2 = linear_interp(y2, y2Interp, interpFrac);
            }
            
            g.drawLine(Line<float>(x1, y1, x2, y2), 3.0);
        }
    }
    
    // paint the border
    g.setColour(getCommonColours().detail);
    g.drawRect(border, 2.0);
}

void WaveViewPanel::resized()
{
    
}

void WaveViewPanel::waveformChanged(float realFrameIndex)
{
    // get the integer part for the index
    frameIndex1 = frameIndex2 = realFrameIndex;
    
    // and the fraction part for the interpolation
    interpFrac = realFrameIndex - frameIndex1;

    // if we have an upper frame to interpolate with...
    if (frameIndex1 + 1 < sound.getWavetable().getNumFrames())
    {
        frameIndex2 = frameIndex1 + 1;
    }
    
    waveform1 = std::make_unique<BandLimitedWaveform>(sound.getWavetable().getFrame(frameIndex1).getWaveform(0));
    waveform2 = std::make_unique<BandLimitedWaveform>(sound.getWavetable().getFrame(frameIndex2).getWaveform(0));
    
    repaint();
}

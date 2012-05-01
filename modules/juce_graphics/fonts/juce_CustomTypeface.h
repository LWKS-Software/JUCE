/*
  ==============================================================================

   This file is part of the JUCE library - "Jules' Utility Class Extensions"
   Copyright 2004-11 by Raw Material Software Ltd.

  ------------------------------------------------------------------------------

   JUCE can be redistributed and/or modified under the terms of the GNU General
   Public License (Version 2), as published by the Free Software Foundation.
   A copy of the license is included in the JUCE distribution, or can be found
   online at www.gnu.org/licenses.

   JUCE is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

  ------------------------------------------------------------------------------

   To release a closed-source product which uses JUCE, commercial licenses are
   available: visit www.rawmaterialsoftware.com/juce for more information.

  ==============================================================================
*/

#ifndef __JUCE_CUSTOMTYPEFACE_JUCEHEADER__
#define __JUCE_CUSTOMTYPEFACE_JUCEHEADER__

#include "juce_Typeface.h"
class InputStream;
class OutputStream;


//==============================================================================
/**
    A typeface that can be populated with custom glyphs.

    You can create a CustomTypeface if you need one that contains your own glyphs,
    or if you need to load a typeface from a Juce-formatted binary stream.

    If you want to create a copy of a native face, you can use addGlyphsFromOtherTypeface()
    to copy glyphs into this face.

    @see Typeface, Font
*/
class JUCE_API  CustomTypeface  : public Typeface
{
public:
    //==============================================================================
    /** Creates a new, empty typeface. */
    CustomTypeface();

    /** Loads a typeface from a previously saved stream.
        The stream must have been created by writeToStream().
        @see writeToStream
    */
    explicit CustomTypeface (InputStream& serialisedTypefaceStream);

    /** Destructor. */
    ~CustomTypeface();

    //==============================================================================
    /** Resets this typeface, deleting all its glyphs and settings. */
    void clear();

    /** Sets the vital statistics for the typeface.
        @param fontFamily the typeface's font family
        @param ascent     the ascent - this is normalised to a height of 1.0 and this is
                          the value that will be returned by Typeface::getAscent(). The
                          descent is assumed to be (1.0 - ascent)
        @param isBold     should be true if the typeface is bold
        @param isItalic   should be true if the typeface is italic
        @param defaultCharacter   the character to be used as a replacement if there's
                          no glyph available for the character that's being drawn
    */
    void setCharacteristics (const String& fontFamily, float ascent,
                             bool isBold, bool isItalic,
                             juce_wchar defaultCharacter) noexcept;

    /** Sets the vital statistics for the typeface.
        @param fontFamily the typeface's font family
        @param fontStyle  the typeface's font style
        @param ascent     the ascent - this is normalised to a height of 1.0 and this is
                          the value that will be returned by Typeface::getAscent(). The
                          descent is assumed to be (1.0 - ascent)
        @param defaultCharacter  the character to be used as a replacement if there's
                          no glyph available for the character that's being drawn
    */
    void setCharacteristics (const String& fontFamily, const String& fontStyle,
                             float ascent, juce_wchar defaultCharacter) noexcept;

    /** Adds a glyph to the typeface.

        The path that is passed in is normalised so that the font height is 1.0, and its
        origin is the anchor point of the character on its baseline.

        The width is the nominal width of the character, and any extra kerning values that
        are specified will be added to this width.
    */
    void addGlyph (juce_wchar character, const Path& path, float width) noexcept;

    /** Specifies an extra kerning amount to be used between a pair of characters.
        The amount will be added to the nominal width of the first character when laying out a string.
    */
    void addKerningPair (juce_wchar char1, juce_wchar char2, float extraAmount) noexcept;

    /** Adds a range of glyphs from another typeface.
        This will attempt to pull in the paths and kerning information from another typeface and
        add it to this one.
    */
    void addGlyphsFromOtherTypeface (Typeface& typefaceToCopy, juce_wchar characterStartIndex, int numCharacters) noexcept;

    /** Saves this typeface as a Juce-formatted font file.
        A CustomTypeface can be created to reload the data that is written - see the CustomTypeface
        constructor.
    */
    bool writeToStream (OutputStream& outputStream);

    //==============================================================================
    // The following methods implement the basic Typeface behaviour.
    float getAscent() const;
    float getDescent() const;
    float getStringWidth (const String& text);
    void getGlyphPositions (const String& text, Array <int>& glyphs, Array<float>& xOffsets);
    bool getOutlineForGlyph (int glyphNumber, Path& path);
    EdgeTable* getEdgeTableForGlyph (int glyphNumber, const AffineTransform& transform);

protected:
    //==============================================================================
    juce_wchar defaultCharacter;
    float ascent;

    //==============================================================================
    /** If a subclass overrides this, it can load glyphs into the font on-demand.
        When methods such as getGlyphPositions() or getOutlineForGlyph() are asked for a
        particular character and there's no corresponding glyph, they'll call this
        method so that a subclass can try to add that glyph, returning true if it
        manages to do so.
    */
    virtual bool loadGlyphIfPossible (juce_wchar characterNeeded);

private:
    //==============================================================================
    class GlyphInfo;
    friend class OwnedArray<GlyphInfo>;
    OwnedArray <GlyphInfo> glyphs;
    short lookupTable [128];

    GlyphInfo* findGlyph (const juce_wchar character, bool loadIfNeeded) noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CustomTypeface);
};

#endif   // __JUCE_CUSTOMTYPEFACE_JUCEHEADER__

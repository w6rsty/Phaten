const vec2 FontSheetSize = vec2(160.0, 160.0);
const float FontWidth = 8.0;
const float FontHeight = 8.0;

vec2 GetFontTexPos(float fontIndex, float index)
{
    float x = float(int(fontIndex) % 10);
    float y = float(int(fontIndex) / 10);
    float corner = int(index) % 4;

    float xOffset = (corner == 1 || corner == 2) ? x + 1 : x;
    float yOffset = (corner == 2 || corner == 3) ? y + 1 : y;

    return vec2(xOffset * FontWidth, yOffset * FontHeight) / FontSheetSize;
}


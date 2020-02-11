#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include "catch.hpp"

#include "utils.h"

#include "color_space.h"

TEST_CASE("Color space test") {
    ColorSpace* render = new ColorSpace(1920, 1080);

    render->Clear();

    BENCHMARK("Draw scene") 
    {
        render->DrawScene();
    };
    
    // Color_space.png is a bit too precise? Starting from x=143, y=0, every 15 pixels the G element of the image is off by 1 on the result
    REQUIRE(validate_framebuffer("references/color_space.png", render->GetFrameBuffer()));
}
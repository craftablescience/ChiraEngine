#pragma once

// IMPORTANT: When creating a new material engine-side, add it here so it's registered with the material factory!
// If you are making your own material, you'll need to add it here so it works in the model viewer, but if you
// don't care about that then you can ignore this file.

#include <render/material/materialCubemap.h>
#include <render/material/materialPhong.h>
#include <render/material/materialTextured.h>
#include <render/material/materialUntextured.h>

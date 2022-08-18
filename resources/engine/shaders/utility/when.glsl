/*
Taken from https://theorangeduck.com/page/avoiding-shader-conditionals

Old and slow:

if (x == 0) {
    y += 5;
}

New and fast:

y += 5 * when_eq(x, 0)

*/

vec4 when_eq(vec4 x, vec4 y) {
    return 1.0 - abs(sign(x - y));
}

vec4 when_neq(vec4 x, vec4 y) {
    return abs(sign(x - y));
}

vec4 when_gt(vec4 x, vec4 y) {
    return max(sign(x - y), 0.0);
}

vec4 when_lt(vec4 x, vec4 y) {
    return max(sign(y - x), 0.0);
}

vec4 when_ge(vec4 x, vec4 y) {
    return 1.0 - when_lt(x, y);
}

vec4 when_le(vec4 x, vec4 y) {
    return 1.0 - when_gt(x, y);
}

vec4 when_and(vec4 a, vec4 b) {
    return a * b;
}

vec4 when_or(vec4 a, vec4 b) {
    return min(a + b, 1.0);
}

vec4 when_xor(vec4 a, vec4 b) {
    return mod(a + b, 2.0);
}

vec4 when_not(vec4 a) {
    return 1.0 - a;
}

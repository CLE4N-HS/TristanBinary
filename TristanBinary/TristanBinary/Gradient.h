#include <math.h>

class Vector2f
{
public:
    Vector2f(float x, float y) { this->x = x; this->y = y; }

    float x, y;
    Vector2f operator/ (float a) { return Vector2f(x / a, y / a); }
    Vector2f operator+ (Vector2f b) { return Vector2f(x + b.x, y + b.y); }
    Vector2f operator- (Vector2f b) { return Vector2f(x - b.x, y - b.y); }
    Vector2f operator* (Vector2f b) { return Vector2f(x * b.x, y * b.y); }
    Vector2f operator* (float a) { return Vector2f(x * a, y * a); }
    static float Dot(const Vector2f a, const Vector2f b) { return a.x * b.x + a.y * b.y; }
    float Length() { return sqrtf(x * x + y * y); }
    static Vector2f Normalize(Vector2f a) { if (a.Length() < 0.0001f) return Vector2f(0.f, 0.f); return a / a.Length(); }
};

class GradientNoise
{
private:
    static Vector2f GradNoiseDir(Vector2f p)
    {
        float pxmod = fmod(p.x, 289.f);
        p = Vector2f(pxmod, fmod(p.y, 289.f));
        float x = (34.f * p.x + 1.f) * pxmod + p.y;
        x = (34.f * x + 1.f) * pxmod;
        x = ((x / 41.f) - (long)(x / 41)) * 2.f - 1.f;
        return Vector2f::Normalize(Vector2f(x - floorf(x + 0.5f), fabsf(x) - 0.5f));
    }

    static float smoothStep(float edge0, float edge1, float x) {
        x = fmax(0.0f, fmin(1.0f, (x - edge0) / (edge1 - edge0)));
        return x * x * (3 - 2 * x);  // Fonction de lissage
    }

    static float Lerp(float a, float b, float t) { return (b - a) * t + a; }

    static float GradNoise(Vector2f p)
    {
        Vector2f ip = Vector2f(floorf(p.x), floorf(p.y));
        Vector2f fp = Vector2f(p.x - ((long)p.x), p.y - ((long)p.y));
        float d00 = Vector2f::Dot(GradNoiseDir(ip), fp);
        float d01 = Vector2f::Dot(GradNoiseDir(ip + Vector2f(0.f, 1.f)), fp - Vector2f(0.f, 1.f));
        float d10 = Vector2f::Dot(GradNoiseDir(ip + Vector2f(1.f, 0.f)), fp - Vector2f(1.f, 0.f));
        float d11 = Vector2f::Dot(GradNoiseDir(ip + Vector2f(1.f, 1.f)), fp - Vector2f(1.f, 1.f));
        fp = fp * fp * fp * (fp * (fp * 6.f - Vector2f(15.f, 15.f)) + Vector2f(10.f, 10.f));
        return Lerp(Lerp(d00, d01, fp.y), Lerp(d10, d11, fp.y), fp.x);
    }
public:
    static float GradNoise(Vector2f uv, float scale)
    {
        return clamp(-1.f, 1.0f, GradNoise(uv * scale)) * 2.f;
        return clamp(0.f, 1.0f, GradNoise(uv * scale) + 0.5f);
    }
    static float clamp(float min, float max, float value)
    {
        return fmax(min, fmin(max, value));
    }

};
using System;
using System.Collections.Generic;
using System.Globalization;

public static class CustomExtension {
    public static int? ToInt32(this string numeric) {
        if (int.TryParse(numeric, NumberStyles.Any, CultureInfo.InvariantCulture, out var ret)) {
            return ret;
        }

        return null;
    }
    
    public static long? ToInt64(this string numeric) {
        if (long.TryParse(numeric, NumberStyles.Any, CultureInfo.InvariantCulture, out var ret)) {
            return ret;
        }

        return null;
    }

    public static int ToInt32(this string numeric, int defaultValue)
        => numeric.ToInt32() ?? defaultValue;

    public static float? ToFloat(this string floatValue) {
        if (float.TryParse(floatValue, NumberStyles.Any, CultureInfo.InvariantCulture, out var ret)) {
            return ret;
        }

        return null;
    }
    
    public static float ToFloat(this string floatValue, float defaultValue)
        => floatValue.ToFloat() ?? defaultValue;
    
    public static float CalculateDiffRate(this float origin, float target, float coefficient = 1f, float max = float.MaxValue, float min = 1f) {
        if (origin == 0f || target == 0f) return 0f;

        float diffRate = (origin > target ? origin / target : target / origin) - 1f;
        return (diffRate * coefficient).Clamp(min, max);
    }

    public static float CalculateDiffRate(this float? origin, float? target, float coefficient = 1f, float max = float.MaxValue, float min = 1f) {
        if (origin == null || target == null) return 0f;
        return ((float)origin).CalculateDiffRate((float)target, coefficient, max, min);
    }

    public static float Clamp(this float origin, float min, float max) {
        if (origin < min) return min;
        if (origin > max) return max;
        return origin;
    }
    
    public static int Clamp(this int origin, int min, int max) {
        if (origin < min) return min;
        if (origin > max) return max;
        return origin;
    }

    public static void SortByCompanyPoint(this List<Company> list) {
        list.Sort((c1, c2) => {
            int score1 = c1.RecommendPoint - c1.WarningPoint;
            int score2 = c2.RecommendPoint - c2.WarningPoint;
            return score2 - score1;
        });
    }
}
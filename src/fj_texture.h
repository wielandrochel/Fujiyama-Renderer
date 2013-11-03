/*
Copyright (c) 2011-2013 Hiroshi Tsubokawa
See LICENSE and README
*/

#ifndef FJ_TEXTURE_H
#define FJ_TEXTURE_H

#ifdef __cplusplus
extern "C" {
#endif

struct Texture;
struct Color4;

extern struct Texture *TexNew(void);
extern void TexFree(struct Texture *tex);

/*
 * Looks up a value of texture image.
 * (r, r, r, 1) will be returned when texture is grayscale.
 * (r, g, b, 1) will be returned when texture is rgb.
 * (r, g, b, a) will be returned when texture is rgba.
 */
extern void TexLookup(const struct Texture *tex, float u, float v, struct Color4 *rgba);
extern int TexLoadFile(struct Texture *tex, const char *filename);

extern int TexGetWidth(const struct Texture *tex);
extern int TexGetHeight(const struct Texture *tex);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* FJ_XXX_H */

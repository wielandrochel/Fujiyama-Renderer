/*
Copyright (c) 2011-2014 Hiroshi Tsubokawa
See LICENSE and README
*/

#ifndef FJ_PLUGIN_H
#define FJ_PLUGIN_H

#include <string>
#include <cstddef>

#define PLUGIN_API_VERSION 1

namespace fj {

struct Plugin;
struct PluginInfo;
struct Property;
struct MetaInfo;

typedef int (*PlgInitializeFn)(struct PluginInfo *info);
typedef void *(*PlgCreateInstanceFn)(void);
typedef void (*PlgDeleteInstanceFn)(void *obj);

enum PlgErrorNo {
  PLG_ERR_NONE = 0,
  PLG_ERR_PLUGIN_NOT_FOUND,
  PLG_ERR_INIT_PLUGIN_FUNC_NOT_EXIST,
  PLG_ERR_INIT_PLUGIN_FUNC_FAIL,
  PLG_ERR_BAD_PLUGIN_INFO,
  PLG_ERR_CLOSE_PLUGIN_FAIL,
  PLG_ERR_NO_MEMORY
};

class PluginInfo {
public:
  PluginInfo() :
      api_version(0),
      plugin_type(NULL),
      plugin_name(NULL),
      create_instance(NULL),
      delete_instance(NULL),
      vtbl(NULL),
      properties(NULL),
      meta(NULL)
  {}
  ~PluginInfo() {}
  
  int api_version;
  const char *plugin_type;
  const char *plugin_name;
  PlgCreateInstanceFn create_instance;
  PlgDeleteInstanceFn delete_instance;
  const void *vtbl;
  const struct Property *properties;
  const struct MetaInfo *meta;
};

class MetaInfo {
public:
#if 0
  MetaInfo() :
      name(NULL),
      data(NULL)
  {}
  ~MetaInfo() {}
#endif

  const char *name;
  const char *data;
};

class Plugin {
public:
  Plugin();
  ~Plugin();

  int Open(const std::string &filename);
  void Close();

  void *CreateInstance() const;
  void DeleteInstance(void *instance) const;

  const Property *GetPropertyList() const;
  const MetaInfo *Metainfo() const;
  const void *GetVtable() const;
  const char *GetName() const;
  const char *GetType() const;
  int TypeMatch(const char *type) const;

public:
  void *dso_;
  PluginInfo info_;
};

extern struct Plugin *PlgOpen(const char *filename);
extern int PlgClose(struct Plugin *plugin);

extern void *PlgCreateInstance(const struct Plugin *plugin);
extern void PlgDeleteInstance(const struct Plugin *plugin, void *obj);

extern const struct Property *PlgGetPropertyList(const struct Plugin *plugin);
extern const struct MetaInfo *PlgMetainfo(const struct Plugin *plugin);
extern const void *PlgGetVtable(const struct Plugin *plugin);
extern const char *PlgGetName(const struct Plugin *plugin);
extern const char *PlgGetType(const struct Plugin *plugin);
extern int PlgTypeMatch(const struct Plugin *plugin, const char *type);

extern int PlgSetupInfo(struct PluginInfo *info,
    int api_version,
    const char *plugin_type,
    const char *plugin_name,
    PlgCreateInstanceFn create_instance,
    PlgDeleteInstanceFn delete_instance,
    const void *vtbl,
    const struct Property *properties,
    const struct MetaInfo *meta);

extern int PlgGetErrorNo(void);

} // namespace xxx

#endif // FJ_XXX_H

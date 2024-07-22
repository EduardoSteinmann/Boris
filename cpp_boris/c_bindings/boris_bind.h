#ifndef BORIS_BIND
#define BORIS_BIND

#define BORIS_LANG_GLOBAL_VALUE_EXPOSE(name, data_type)
#define BORIS_LANG_STRUCT_EXPOSE(name, members, member_data_types)
#define BORIS_LANG_UNION_EXPOSE(name, members, member_data_types)
#define BORIS_LANG_ENUM_EXPOSE(name, backing_data_type, values)
#define BORIS_LANG_TYPEDEF_EXPOSE(name, backing_data_type)
#define BORIS_LANG_FUNCTION_EXPOSE(name, return_type, args, args_data_types)

#define BORIS_LANG_BIND(globals, functions)

#endif
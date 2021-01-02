

file(WRITE ${plugin_name}Wrapper.cs
"\
namespace Managed\n\
{\n\
    [Plugin]\n\
    public class ${plugin_name}Wrapper : ${plugin_name}.${plugin_name}, IPlugin {}\n\
}\n\
")

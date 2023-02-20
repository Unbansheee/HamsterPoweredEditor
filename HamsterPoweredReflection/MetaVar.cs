namespace HamsterPoweredReflection;

public class MetaVar
{
    public string Name { get; set; }
    public object DefaultValue { get; set; }
    public string Type { get; set; }
    public Access Access { get; set; }
    public bool IsStatic { get; set; }
    public bool IsConst { get; set; }
}
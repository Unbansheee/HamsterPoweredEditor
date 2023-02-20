using System.Text;

namespace HamsterPoweredReflection;

public class MetaFunction
{
    public string Name { get; set; }
    public bool IsStatic { get; set; }
    public Access Access { get; set; }
    public bool IsVirtual { get; set; }
    public string ReturnType { get; set; }
    public List<MetaParameter> Parameters { get; set; } = new List<MetaParameter>();

    public string GetSignature()
    {
        var sb = new StringBuilder();
        sb.Append(Name);
        sb.Append("(");
        foreach (var parameter in Parameters)
        {
            sb.Append(parameter.Type);
            sb.Append(" ");
            sb.Append(parameter.Name);
            if (parameter.DefaultValue != null)
            {
                sb.Append(" = ");
                sb.Append(parameter.DefaultValue);
            }
            sb.Append(",");
        }
        sb.Append(")");
        return sb.ToString();
    }
}
struct Input
{
    float3 Position : POSITION;
    float4 Color : COLOR;
};

struct Output
{
    float4 Color : COLOR;
    float4 Position : SV_Position;
};

Output main(Input input)
{
    Output output;
    output.Color = input.Color;
    output.Position = float4(input.Position, 1.0f);
    return output;
}

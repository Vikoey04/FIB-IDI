#version 330 core

//in vec3 fcolor;
in vec4 vertexFS;
in vec3 normalFS;

in vec3  matambFS;
in vec3  matdiffFS;
in vec3  matspecFS;
in float matshinFS;

out vec4 FragColor;

uniform vec3 posFocus;
uniform vec3 colFocus;
uniform vec3 llumAmbient;

vec3 Ambient() {
    return llumAmbient * matambFS;
}

vec3 Difus (vec3 NormSCO, vec3 L, vec3 colFocus)
{
    // Fixeu-vos que SOLS es retorna el terme difús
    // S'assumeix que els vectors que es reben com a paràmetres estan normalitzats
    vec3 colRes = vec3(0);
    // Càlcul component difusa, si n'hi ha
    if (dot (L, NormSCO) > 0)
      colRes = colFocus * matdiffFS * dot (L, NormSCO);//llumAmbient = vec3(0,0,0);
    return (colRes);
}

vec3 Especular (vec3 NormSCO, vec3 L, vec4 vertSCO, vec3 colorFocus)
{
    // Fixeu-vos que SOLS es retorna el terme especular!
    // Assumim que els vectors estan normalitzats
    vec3 colRes = vec3 (0);
    // Si la llum ve de darrera o el material és mate no fem res
    if ((dot(NormSCO,L) < 0) || (matshinFS == 0))
      return colRes;  // no hi ha component especular

    // Calculem R i V
    vec3 R = reflect(-L, NormSCO); // equival a: 2.0*dot(NormSCO,L)*NormSCO - L;
    vec3 V = normalize(-vertSCO.xyz); // perquè la càmera està a (0,0,0) en SCO

    if (dot(R, V) < 0)
      return colRes;  // no hi ha component especular

    float shine = pow(max(0.0, dot(R, V)), matshinFS);
    return (matspecFS * colorFocus * shine);
    //matshinFS
}

void main()
{
	vec3 LF = posFocus - vertexFS.xyz;
  LF = normalize(LF);

	vec3 normalF = normalize(normalFS);

  vec3 fcolor = Ambient()+Especular(normalF, LF, vertexFS, colFocus);

	FragColor = vec4(fcolor, 1.0);
}

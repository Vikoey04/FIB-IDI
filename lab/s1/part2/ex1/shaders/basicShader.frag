#version 330 core

out vec4 FragColor;

void main() {
    FragColor = vec4(0, 0, 0, 1);

    FragColor = vec4(1.); //blanc
    
      if (gl_FragCoord.x < 354. && gl_FragCoord.y < 354.) { //abaix esquerra
        FragColor = vec4(1., 1., 0., 1);
      }
      if (gl_FragCoord.x > 354. && gl_FragCoord.y < 354.) { //abaix dreta
        FragColor = vec4(0., 1., 0., 1);
      }
      if (gl_FragCoord.x < 354. && gl_FragCoord.y > 354.) { //dalt esquerra
        FragColor = vec4(1., 0., 0., 1);
      }
      if (gl_FragCoord.x > 354. && gl_FragCoord.y > 354.) { //dalt dreta
        FragColor = vec4(0., 0., 1., 1);
      }

}

#version 330 core

out vec4 FragColor;

void main() {

    if (int(gl_FragCoord.y) % 28 <= 13) {
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
    else FragColor = vec4(0.5, 0.7, 1.0, 1.0);
}

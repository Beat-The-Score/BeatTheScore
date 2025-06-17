import QtQuick 2.0

Rectangle {
    width: mainGame.isDesktop() ? parent.width : 0
    height: mainGame.isDesktop() ? parent.height : 0
    property variant source;

    property bool active: mainGame.isDesktop()// && !mainGame.pauseMenuVisible

    ShaderEffectSource {
        id: shaderSource
        sourceItem: source
        mipmap: mainGame.isDesktop() ? true : false
    }

    ShaderEffect {
        id: shader
        anchors.fill: parent
        property variant source: shaderSource
        property size delta: Qt.size(1.0 / width, 1.0 / height)
        visible: active
        fragmentShader: !mainGame.isDesktop() ? "" : "
uniform sampler2D source;
uniform highp vec2 delta;
varying highp vec2 qt_TexCoord0;
//highp int r = 8, bias = 4;
highp int r = 2, bias = 3;

void main() {

    gl_FragColor = min(texture2D(source, qt_TexCoord0) , (
        texture2D(source, qt_TexCoord0 + delta * vec2(r, r), bias) +
        texture2D(source, qt_TexCoord0 + delta * vec2(r, -r), bias) +
        texture2D(source, qt_TexCoord0 + delta * vec2(-r, r), bias) +
        texture2D(source, qt_TexCoord0 + delta * vec2(-r, -r), bias)) *
        vec4(vec3(.25 * .5), 1) + vec4(vec3(.5), 0));
/*
    for (int i=-3; i<=3; i+=2)
        for (int j=-3; j<=3; j+=2)
            gl_FragColor += texture2D(source, qt_TexCoord0 + delta * vec2(i * r, j * r), bias);
    gl_FragColor = min(texture2D(source, qt_TexCoord0) ,
        gl_FragColor * vec4(vec3(.0625 * .5), 1) + vec4(vec3(.5), 0));*/
    //gl_FragColor *= vec4(vec3(.0625), 1);
    //gl_FragColor = (texture2D(source, qt_TexCoord0) + gl_FragColor);
    gl_FragColor.a = 1.0;
}
"
    }
}

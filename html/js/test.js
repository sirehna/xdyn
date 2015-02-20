if ( ! Detector.webgl ) Detector.addGetWebGLMessage();

var container, stats;

var camera, cameraTarget, scene, renderer;
var meshes = {};

init();
animate();
/*
if ('WebSocket' in window){
    console.log("websocket supported");
  /* WebSocket is supported. You can proceed with your code
    var connection = new WebSocket('ws://localhost:12345/test');

} else {
console.log("websocket NOT supported");
   /*WebSockets are not supported. Try a fallback method like long-polling etc
}
*/

function init() {

    container = document.createElement( 'div' );
    document.body.appendChild( container );

    camera = new THREE.PerspectiveCamera( 35, window.innerWidth / window.innerHeight, 1, 15 );
    camera.position.set( 3, 0.15, 3 );
    camera.position.x = 2;/*Math.cos( timer ) * 3;*/
    camera.position.y = 2;/*Math.sin( timer ) * 3;*/
    camera.position.z = 2;/*Math.sin( timer ) * 3;*/

    cameraTarget = new THREE.Vector3( 0, -0.25, 0 );
    camera.lookAt( cameraTarget );

    scene = new THREE.Scene();
    scene.fog = new THREE.Fog( 0x72645b, 2, 15 );


    // Ground
    var plane = new THREE.Mesh(
        new THREE.PlaneBufferGeometry( 40, 40 ),
        new THREE.MeshPhongMaterial( { ambient: 0x999999, color: 0x999999, specular: 0x101010 } )
    );
    plane.rotation.x = -Math.PI/2;
    plane.position.y = -0.5;
    scene.add( plane );

    plane.receiveShadow = true;
    // Binary files

    var material = new THREE.MeshPhongMaterial( { ambient: 0x555555, color: 0xAAAAAA, specular: 0x111111, shininess: 200 } );

    var loader = new THREE.STLLoader();
    loader.load( 'pr2_head_pan.stl', function ( geometry ) {

    var mesh = new THREE.Mesh( geometry, material );
    meshes["bla"] = mesh;
    mesh.position.set( 0, - 0.37, - 0.6 );
    mesh.rotation.set( - Math.PI / 2, 0, 0 );
    mesh.scale.set( 2, 2, 2 );

    mesh.castShadow = true;
    mesh.receiveShadow = true;
    mesh.matrixAutoUpdate = true;
    scene.add( mesh );

    } );
    console.log(meshes);

    // Lights
    scene.add( new THREE.AmbientLight( 0x777777 ) );

    addShadowedLight( 1, 1, 1, 0xffffff, 1.35 );
    addShadowedLight( 0.5, 1, -1, 0xffaa00, 1 );

    // renderer

    renderer = new THREE.WebGLRenderer( { antialias: true } );
    renderer.setClearColor( scene.fog.color );
    renderer.setPixelRatio( window.devicePixelRatio );
    renderer.setSize( window.innerWidth, window.innerHeight );

    renderer.gammaInput = true;
    renderer.gammaOutput = true;

    renderer.shadowMapEnabled = true;
    renderer.shadowMapCullFace = THREE.CullFaceBack;

    container.appendChild( renderer.domElement );
}

function addShadowedLight( x, y, z, color, intensity ) {

    var directionalLight = new THREE.DirectionalLight( color, intensity );
    directionalLight.position.set( x, y, z )
    scene.add( directionalLight );

    directionalLight.castShadow = true;
    // directionalLight.shadowCameraVisible = true;

    var d = 1;
    directionalLight.shadowCameraLeft = -d;
    directionalLight.shadowCameraRight = d;
    directionalLight.shadowCameraTop = d;
    directionalLight.shadowCameraBottom = -d;

    directionalLight.shadowCameraNear = 1;
    directionalLight.shadowCameraFar = 4;

    directionalLight.shadowMapWidth = 1024;
    directionalLight.shadowMapHeight = 1024;

    directionalLight.shadowBias = -0.005;
    directionalLight.shadowDarkness = 0.15;
}

function animate() {
    requestAnimationFrame( animate );
    render();
}

function f(x,y,z) {
    /*
    x = meshes["bla"].position.x + x;
    y = meshes["bla"].position.y + y;
    z = meshes["bla"].position.z + z;
    console.log(x);
    console.log(y);
    console.log(z);*/
    render_(x,y,z);
}

function reset()
{
    meshes["bla"].position.x=0;
    meshes["bla"].position.y=0;
    meshes["bla"].position.z=0;
}
function render_(x,y,z) {
if ("bla" in meshes)
{
    console.log("x = "+x);
    console.log("y = "+y);
    console.log("z = "+z);
    meshes["bla"].position.x=x;
    meshes["bla"].position.y=y;
    meshes["bla"].position.z=z;
}
    renderer.render( scene, camera );

}

function render() {
    var timer = 0;/*Date.now() * 5e-3;*/
    if ("bla" in meshes)
    {

        var x = meshes["bla"].position.x;
        var y = meshes["bla"].position.y;
        var z = meshes["bla"].position.z;
        render_(x,y,z);
    }
}

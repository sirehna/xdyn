if ( ! Detector.webgl ) Detector.addGetWebGLMessage();

var container;
var bbox;
var camera, cameraTarget, cameraPosition, scene, renderer;
var meshes = {};
var bboxes = {};

var current = { c: 1};

init();
console.log("Start animate");
animate();

function resetCamera() {
    camera.position.set( -0.0, +0.0, -1.0);
    camera.lookAt( cameraTarget );
}

function sleep(miliseconds) {
    var currentTime = new Date().getTime();
    while (currentTime + miliseconds >= new Date().getTime()) {
    }
}

function init() {
    console.log("Init");
    container = document.createElement( 'div' );
    document.body.appendChild( container );

    camera = new THREE.PerspectiveCamera( 35, window.innerWidth / window.innerHeight, 1, 15 );
    cameraPosition = new THREE.Vector3( 0, 0.0, 0 );
    cameraTarget = new THREE.Vector3( 0, 0.0, 0 );
    resetCamera()

    controls = new THREE.TrackballControls( camera );
    controls.rotateSpeed = 1.0;
    controls.zoomSpeed = 1.2;
    controls.panSpeed = 0.8;
    controls.noZoom = false;
    controls.noPan = false;
    controls.staticMoving = true;
    controls.dynamicDampingFactor = 0.3;
    controls.keys = [ 65, 83, 68 ];
    controls.addEventListener( 'change', render );

    scene = new THREE.Scene();
    // Fog
    //scene.fog = new THREE.Fog( 0x72645b, 2, 15 );

    var cc = function(scope)
    {
        scene.add( scope.bodyMesh );
        // Grid
        var dx = (scope.bodyBBox.max.x-scope.bodyBBox.min.x);
        var dy = (scope.bodyBBox.max.y-scope.bodyBBox.min.y);
        var dz = (scope.bodyBBox.max.z-scope.bodyBBox.min.z);
        var mx = 0.5*(scope.bodyBBox.max.x+scope.bodyBBox.min.x);
        var my = 0.5*(scope.bodyBBox.max.y+scope.bodyBBox.min.y);
        var mz = 0.5*(scope.bodyBBox.max.z+scope.bodyBBox.min.z);
        cameraTarget.x=mx;
        cameraTarget.y=my;
        cameraTarget.z=mz;
        cameraPosition.x=mx-dx;
        cameraPosition.y=my-dy;
        cameraPosition.z=mz-dz;
        camera.lookAt( cameraTarget );
        camera.position.set(cameraPosition);
        console.log("Hello")
        console.log(camera.position.x.x)
        console.log(camera.position.x.y)
        console.log(camera.position.x.z)
        console.log(cameraTarget.x)
        console.log(cameraTarget.y)
        console.log(cameraTarget.z)
        addShadowedLight( cameraTarget.x, cameraTarget.y, cameraTarget.z-1, 0xffffff, 135 );
        //addShadowedLight( 0.5, 1, -1, 0xffaa00, 1 );
        var pGrid = 10*0.5*dx;
        var geometry = new THREE.Geometry();
        geometry.vertices.push( new THREE.Vector3( - pGrid, 0, 0 ) );
        geometry.vertices.push( new THREE.Vector3( pGrid, 0, 0 ) );
        linesMaterial = new THREE.LineBasicMaterial( 0x000000, 0.2 );
        for ( var i = 0; i <= 20; i ++ ) {
            var line = new THREE.Line( geometry, linesMaterial );
            line.position.z = ( i * pGrid/10.0 ) - pGrid;
            scene.add( line );
            var line = new THREE.Line( geometry, linesMaterial );
            line.position.x = ( i * pGrid/10.0 ) - pGrid;
            line.rotation.y = 90 * Math.PI / 180;
            scene.add( line );
        }

        // Ground
        var plane = new THREE.Mesh(
            new THREE.PlaneBufferGeometry( 3*pGrid, 3*pGrid ),
            new THREE.MeshPhongMaterial( { ambient: 0x999999, color: 0x999999, specular: 0x101010 } )
        );
        plane.rotation.x = -Math.PI/2;
        plane.position.y = 0.0; //-0.5;
        plane.receiveShadow = true;
        scene.add( plane );
    };
//    c = new Car();
//    c.callback = cc
//    c.loadPartsSTL('stl/anthineas.stl');

    var plane = new THREE.Mesh(
            new THREE.PlaneBufferGeometry( 3*10, 3*10),
            new THREE.MeshPhongMaterial( { ambient: 0x999999, color: 0x999999, specular: 0x101010 } )
        );
        plane.rotation.x = -Math.PI/2;
        plane.position.y = 0.0; //-0.5;
        plane.receiveShadow = true;
        scene.add( plane );

//        var plane = new THREE.Mesh(
//                new THREE.PlaneBufferGeometry( 3*10, 3*10),
//                new THREE.MeshPhongMaterial( { ambient: 0x999999, color: 0x999999, specular: 0x101010 } )
//            );
//            plane.rotation.x = +Math.PI/2;
//            plane.position.y = 0.0; //-0.5;
//            plane.receiveShadow = true;
//            scene.add( plane );

    // Lights
        hemiLight = new THREE.HemisphereLight( 0xffffff, 0xffffff, 0.6 );
        hemiLight.color.setHSL( 0.6, 1, 0.6 );
        hemiLight.groundColor.setHSL( 0.095, 1, 0.75 );
        hemiLight.position.set( 0, 0, -10 );
        scene.add( hemiLight );

    var ambLight = new THREE.AmbientLight(0x404040);
    scene.add(ambLight);

    addShadowedLight( 1, 1, 1, 0xffffff, 1.35 );
    addShadowedLight( 0.5, 1, -1, 0xffaa00, 1 );

    // renderer
    renderer = new THREE.WebGLRenderer( { antialias: true } );
    //GJ  renderer.setClearColor( scene.fog.color );
    renderer.setPixelRatio( window.devicePixelRatio );
    renderer.setSize( window.innerWidth, window.innerHeight );
//    renderer.gammaInput = true;
//    renderer.gammaOutput = true;
//    renderer.shadowMapEnabled = true;
//    renderer.shadowMapCullFace = THREE.CullFaceBack;

    container.appendChild( renderer.domElement );
}

function addShadowedLight( x, y, z, color, intensity ) {

    var directionalLight = new THREE.DirectionalLight( color, intensity );
    directionalLight.position.set( x, y, z )
    scene.add( directionalLight );
    directionalLight.castShadow = true;
//    // directionalLight.shadowCameraVisible = true;
//    var d = 1;
//    directionalLight.shadowCameraLeft = -d;
//    directionalLight.shadowCameraRight = d;
//    directionalLight.shadowCameraTop = d;
//    directionalLight.shadowCameraBottom = -d;
//    directionalLight.shadowCameraNear = 1;
//    directionalLight.shadowCameraFar = 4;
//    directionalLight.shadowMapWidth = 1024;
//    directionalLight.shadowMapHeight = 1024;
//    directionalLight.shadowBias = -0.005;
//    directionalLight.shadowDarkness = 0.15;
}

function onWindowResize() {
    camera.aspect = window.innerWidth / window.innerHeight;
    camera.updateProjectionMatrix();
    renderer.setSize( window.innerWidth, window.innerHeight );
    controls.handleResize();
    render();
}

function animate() {
    requestAnimationFrame( animate );
    controls.update();
    render();
}

function reset()
{
    for (var mesh in meshes)
    {
        if (meshes.hasOwnProperty(mesh))
        {
            meshes[mesh].position.x=0;
            meshes[mesh].position.y=0;
            meshes[mesh].position.z=0;
            var quaternion = new THREE.Quaternion(1.0,0.0,0.0,0.0);
            meshes[mesh].rotation.setFromQuaternion(quaternion);
            meshes[mesh].updateMatrix();
        }
    }
}

function render_(meshName, x, y, z, qr, qx, qy, qz) {
    meshName = meshName || "anthineas.stl"
    if (meshName in meshes)
    {
        qr = qr || 1.0;
        qx = qx || 0.0;
        qy = qy || 0.0;
        qz = qz || 0.0;
        meshes[meshName].position.x = x;
        meshes[meshName].position.y = y;
        meshes[meshName].position.z = z;
        var quaternion = new THREE.Quaternion(qx,qy,qz,qr);
        quaternion = quaternion.normalize();
        meshes[meshName].rotation.setFromQuaternion(quaternion);
        meshes[meshName].updateMatrix();
    }
}

function getBaseName(str)
{
   var base = new String(str).substring(str.lastIndexOf('/') + 1);
    if(base.lastIndexOf(".") != -1)
        base = base.substring(0, base.lastIndexOf("."));
   return base;
}

function getExtension(filename)
{
   return filename.split('.').pop();
}

function render() {
    renderer.render( scene, camera );
}

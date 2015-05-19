if ( ! Detector.webgl ) Detector.addGetWebGLMessage();

var camera, scene, renderer;
var geometry, points = [];

var windowHalfX = window.innerWidth  / 2;
var windowHalfY = window.innerHeight / 2;

// parameters
var SEPARATION     = 2.5;
var AMOUNTX        = 500;
var AMOUNTY        = 500;
var NNODES         = (AMOUNTX+1)*(AMOUNTY+1);
var WAVE_HEIGHT    = 10;
var WAVE_SPEED     = 0.2;
var ROTATION_SPEED = 0.1;
var DAMP_SPEED     = 0.005;
var CAMERA_SPEED   = 0.05;

var counter = 0;
var rotation = 0;
var mouseX   = -300;
var mouseY   = -300;
var initial_camera_zpos = 500;
var initial_camera_ypos = 500;

init();
animate();

function init() {
  var container = document.createElement('div');
  document.body.appendChild(container);

  camera = new THREE.PerspectiveCamera(75, window.innerWidth / window.innerHeight, 1, 10000);
  camera.position.z = initial_camera_zpos;
  camera.position.y = initial_camera_ypos;

  scene = new THREE.Scene();

  var width  = SEPARATION * AMOUNTX;
  var height = SEPARATION * AMOUNTY;

  geometry = new THREE.PlaneBufferGeometry(width, height, AMOUNTX, AMOUNTY);
  calculateInitialPoints();
  updatePoints();

  var ground = new THREE.Mesh(geometry);
  ground.rotation.x = Math.PI / -2;
  scene.add(ground);
  var wireframe = new THREE.WireframeHelper( ground );
  scene.add( wireframe );

  //renderer = new THREE.CanvasRenderer();
  renderer = new THREE.WebGLRenderer( { antialias: true } );
  renderer.setSize(window.innerWidth, window.innerHeight);
  container.appendChild(renderer.domElement);

  document.addEventListener('mousemove', onDocumentMouseMove, false);
  document.addEventListener('mousedown', onMouseDown, false);
  window.addEventListener('resize', onWindowResize, false);
}

function calculateInitialPoints()
{
    position = geometry.attributes.position.array;
    for(var i = 0; i < NNODES; i++)
    {
        var vx = position[3*i+0];
        var vy = position[3*i+1];
        var x = (vx / SEPARATION) * WAVE_SPEED;
        var y = (vy / SEPARATION) * WAVE_SPEED;
        points[i] = WAVE_HEIGHT * (Math.cos(x*x + y*y) / Math.sqrt(x*x + y*y + 0.25));
    }
}

function updatePoints()
{
    position = geometry.attributes.position.array;
    var cosRotation = Math.cos(rotation);
    for(var i = 0; i < NNODES; i++)
    {
        position[3*i+2] = points[i] * cosRotation;
    }
}

function calculateNextParameters()
{
    rotation = 2.0*Math.PI * counter/50.0;
}

function onWindowResize() {
  windowHalfX = window.innerWidth  / 2;
  windowHalfY = window.innerHeight / 2;
  camera.aspect = window.innerWidth / window.innerHeight;
  camera.updateProjectionMatrix();
  renderer.setSize(window.innerWidth, window.innerHeight);
}

function onDocumentMouseMove(event) {
  mouseX = event.clientX - windowHalfX;
  mouseY = event.clientY - windowHalfY;
}

function onMouseDown(event) {}

function animate() {
  requestAnimationFrame(animate);
  geometry.attributes.position.needsUpdate = true;
  render();
}

function render() {
  camera.position.x += ( mouseX - camera.position.x) * CAMERA_SPEED;
  camera.position.y += (-mouseY - camera.position.y) * CAMERA_SPEED;
  camera.lookAt(scene.position);
  updatePoints();
  renderer.render(scene, camera);
  calculateNextParameters();
  counter += 1;
}

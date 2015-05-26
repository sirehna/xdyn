var colors = [ 0xDF1F1F, 0xDFAF1F, 0x80DF1F, 0x1FDF50,
               0x1FDFDF, 0x1F4FDF, 0x7F1FDF, 0xDF1FAF,
               0xEFEFEF, 0x303030 ];

Mobile = function () {

	var scope = this;

	// internal control variables

	this.bodyMesh = null;
	this.bodyGeometry = null;
	this.bodyMaterials = null;
	this.bodyBBox = new THREE.Box3();

	// internal helper variables
	this.loaded = false;
	this.meshes = [];

	this.isLoaded = function()
	{
		return this.loaded;
	}

	this.getBBox = function()
	{
		return this.bodyBBox.min.x;
	}
	
	// API
	this.enableShadows = function ( enable )
	{
		for ( var i = 0; i < this.meshes.length; i ++ )
		{
			this.meshes[ i ].castShadow = enable;
			this.meshes[ i ].receiveShadow = enable;
		}
	};

	this.setVisible = function ( enable )
	{
		for ( var i = 0; i < this.meshes.length; i ++ )
		{
			this.meshes[ i ].visible = enable;
			this.meshes[ i ].visible = enable;
		}
	};

	this.loadPartsJSON = function ( bodyURL )
	{
		var loader = new THREE.JSONLoader();
		loader.load( bodyURL, function( geometry, materials ) { createBody( geometry, materials ) } );
	};

	this.loadPartsSTLOri = function ( bodyURL )
	{
		var loader = new THREE.STLLoader();
		loader.load( bodyURL, function( geometry, materials ) { createBody( geometry, materials ) } );
	}

	this.loadPartsSTL = function ( bodyURL, callback)
	{
		var loc_loaded = false
		var loader = new THREE.STLLoader();
		var callback = callback || function( geometry, materials ) {
			createBody( geometry, materials );
		};
		loader.load( bodyURL, callback)
	};

	// internal helper methods
	function createBody ( geometry, materials )
	{
		createMembers ( geometry, materials );
		createMobile();
	}

	function createMembers ( geometry, materials )
	{
		var current=current || {c:1};
		scope.bodyGeometry = geometry;
		if (materials == null)
		{
			scope.bodyMaterials = new THREE.MeshBasicMaterial( colors[ current.c ] );
		}
		else
		{
			scope.bodyMaterials = materials;
		}
	}

	function createMobile()
	{
		if ( scope.bodyGeometry )
		{
			scope.bodyGeometry.computeBoundingBox();
			scope.bodyBBox = scope.bodyGeometry.boundingBox;
			//var bodyFaceMaterial = new THREE.MeshFaceMaterial( );
			console.log(scope.bodyMaterials)
			scope.bodyMesh = new THREE.Mesh( scope.bodyGeometry, scope.bodyMaterials );
			scope.bodyMesh.castShadow = true;
			scope.bodyMesh.receiveShadow = true;
			scope.bodyMesh.matrixAutoUpdate = true;
			// callback
			scope.loaded = true;
			if ( scope.callback )
			{
				scope.callback( scope );
			}
		}
	};
};

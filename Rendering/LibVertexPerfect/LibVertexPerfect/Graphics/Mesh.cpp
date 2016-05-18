#include "stdafx.h"
#include "Mesh.h"
#include "DXGraphics.h"
#include "OBJLoader.h"
#include "WICTextureLoader.h"

Mesh::Mesh() {
	DeviceContext = static_cast<DXGraphics*>(LVP::Graphics)->DeviceContext;
}

Mesh::~Mesh() {
		SAFE_RELEASE( VertexBuffer );
		SAFE_RELEASE( IndexBuffer );
}

OBJMesh::OBJMesh( const std::string & file ) {

	auto Device = static_cast<DXGraphics*>(LVP::Graphics)->Device;

	MeshData* mesh = new MeshData();
	mesh->LoadObj( file );

	// load and organize indices in ranges per drawcall (material)
	//
	std::vector<unsigned> indices;
	size_t i_ofs = 0;

	for ( auto& dc : mesh->drawcalls ) {
		// append the drawcall indices
		for ( auto& tri : dc.tris )
			indices.insert( indices.end(), tri.vi, tri.vi + 3 );

		// create a range
		size_t i_size = dc.tris.size() * 3;
		int mtl_index = dc.mtl_index > -1 ? dc.mtl_index : -1;
		index_ranges.push_back( { i_ofs, i_size, 0, mtl_index } );

		i_ofs = indices.size();
	}

	// vertex array descriptor
	D3D11_BUFFER_DESC vbufferDesc = { 0.0f };
	vbufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbufferDesc.CPUAccessFlags = 0;
	vbufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vbufferDesc.MiscFlags = 0;
	vbufferDesc.ByteWidth = mesh->vertices.size() * sizeof( vertex_t );
	// data resource
	D3D11_SUBRESOURCE_DATA vdata;
	vdata.pSysMem = &(mesh->vertices)[0];

	// create vertex buffer on device using descriptor & data
	HRESULT vhr = Device->CreateBuffer( &vbufferDesc, &vdata, &VertexBuffer );

	// index array descriptor
	D3D11_BUFFER_DESC ibufferDesc = { 0.0f };
	ibufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibufferDesc.CPUAccessFlags = 0;
	ibufferDesc.Usage = D3D11_USAGE_DEFAULT;
	ibufferDesc.MiscFlags = 0;
	ibufferDesc.ByteWidth = indices.size() * sizeof( unsigned );
	// data resource
	D3D11_SUBRESOURCE_DATA idata;
	idata.pSysMem = &indices[0];
	// create index buffer on device using descriptor & data
	HRESULT ihr = Device->CreateBuffer( &ibufferDesc, &idata, &IndexBuffer );

	// copy materials from mesh
	append_materials( mesh->materials );

	// load textures associated with materials to device
	for ( auto& mtl : materials ) {
		HRESULT hr;
		std::wstring wstr; // for conversion from string to wstring

		// Kd_map
		if ( mtl.map_Kd.size() ) {
			wstr = std::wstring( mtl.map_Kd.begin(), mtl.map_Kd.end() );
			hr = DirectX::CreateWICTextureFromFile( Device, DeviceContext, wstr.c_str(), &mtl.map_Kd_Tex, &mtl.map_Kd_TexSRV );
			printf( "loading texture %s - %s\n", mtl.map_Kd.c_str(), SUCCEEDED( hr ) ? "OK" : "FAILED" );
		}

		// Ks_map
		if ( mtl.map_Ks.size() ) {
			wstr = std::wstring( mtl.map_Ks.begin(), mtl.map_Ks.end() );
			hr = DirectX::CreateWICTextureFromFile( Device,  DeviceContext, wstr.c_str(), &mtl.map_Ks_Tex, &mtl.map_Ks_TexSRV );
			printf( "loading texture %s - %s\n", mtl.map_Ks.c_str(), SUCCEEDED( hr ) ? "OK" : "FAILED" );
		}

		// Normal_map
		if ( mtl.map_bump.size() ) {
			wstr = std::wstring( mtl.map_bump.begin(), mtl.map_bump.end() );
			hr = DirectX::CreateWICTextureFromFile( Device, DeviceContext, wstr.c_str(), &mtl.map_Normal_Tex, &mtl.map_Normal_TexSRV );
			printf( "loading texture %s - %s\n", mtl.map_bump.c_str(), SUCCEEDED( hr ) ? "OK" : "FAILED" );
		}

		// Mask
		if ( mtl.map_mask.size() ) {
			wstr = std::wstring( mtl.map_mask.begin(), mtl.map_mask.end() );
			hr = DirectX::CreateWICTextureFromFile( Device, DeviceContext, wstr.c_str(), &mtl.map_Mask_Tex, &mtl.map_Mask_TexSRV );
			printf( "loading texture %s - %s\n", mtl.map_mask.c_str(), SUCCEEDED( hr ) ? "OK" : "FAILED" );
		}
		// other maps here...
	}

	SAFE_DELETE( mesh );
}

void OBJMesh::Render( ShaderProgram& shader ) const {
	//set topology
	DeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	// bind vertex buffer
	UINT32 stride = sizeof( vertex_t ); //  sizeof(float) * 8;
	UINT32 offset = 0;

	// Bind vertex buffer
	DeviceContext->IASetVertexBuffers( 0, 1, &VertexBuffer, &stride, &offset );

	// bind index buffer
	DeviceContext->IASetIndexBuffer( IndexBuffer, DXGI_FORMAT_R32_UINT, 0 );

	// iterate drawcalls
	for ( auto& irange : index_ranges ) {
		// fetch material and bind texture
		const material_t& mtl = materials[irange.MaterialIndex];

		shader.RenderDrawcall(mtl);

		// make the drawcall
		DeviceContext->DrawIndexed( irange.size, irange.start, 0 );
	}
}


void CubeMesh::CreateFace( vec3f p0, vec3f p1, vec3f p2, vec3f p3, vec3f normal ) {
	// populate the index array

	// triangle #1
	indices.push_back( vertices.size() + 0 );
	indices.push_back( vertices.size() + 1 );
	indices.push_back( vertices.size() + 3 );

	// triangle #2
	indices.push_back( vertices.size() + 1 );
	indices.push_back( vertices.size() + 2 );
	indices.push_back( vertices.size() + 3 );

	// Vertices
	vertex_t v0, v1, v2, v3;
	v0.Pos = p0;
	v0.Normal = normal;
	v0.TexCoord = { 0, 0 };
	v1.Pos = p1;
	v1.Normal = normal;
	v1.TexCoord = { 0, 1 };
	v2.Pos = p2;
	v2.Normal = normal;
	v2.TexCoord = { 1, 1 };
	v3.Pos = p3;
	v3.Normal = normal;
	v3.TexCoord = { 1, 0 };

	vertices.push_back( v0 );
	vertices.push_back( v1 );
	vertices.push_back( v2 );
	vertices.push_back( v3 );
}

CubeMesh::CubeMesh( ) {

	// F & B
	CreateFace( { -0.5, 0.5f, 0.5f }, { 0.5, 0.5f, 0.5f }, { 0.5, -0.5f, 0.5f }, { -0.5, -0.5f, 0.5f }, { 0, 0, 1 } );
	CreateFace( { -0.5, -0.5f, -0.5f }, { 0.5, -0.5f, -0.5f }, { 0.5, 0.5f, -0.5f }, { -0.5, 0.5f, -0.5f }, { 0, 0, -1 } );

	// Up && Down
	CreateFace( { -0.5, 0.5f, -0.5f }, { 0.5, 0.5f, -0.5f }, { 0.5, 0.5f, 0.5f }, { -0.5, 0.5f, 0.5f }, { 0, 1, 0 } );
	CreateFace( { -0.5, -0.5f, 0.5f }, { 0.5, -0.5f, 0.5f }, { 0.5, -0.5f, -0.5f }, { -0.5, -0.5f, -0.5f }, { 0, -1, 0 } );

	// L && R
	CreateFace( { -0.5, -0.5f, 0.5f }, { -0.5, -0.5f, -0.5f }, { -0.5, 0.5f, -0.5f }, { -0.5, 0.5f, 0.5f }, { -1, 0, 0 } );
	CreateFace( { 0.5, -0.5f, -0.5f }, { 0.5, -0.5f, 0.5f }, { 0.5, 0.5f, 0.5f }, { 0.5, 0.5f, -0.5f }, { 1, 0, 0 } );

	auto Device = static_cast<DXGraphics*>(LVP::Graphics)->Device;

	// vertex array descriptor
	D3D11_BUFFER_DESC vbufferDesc = { 0.0f };
	vbufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbufferDesc.CPUAccessFlags = 0;
	vbufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vbufferDesc.MiscFlags = 0;
	vbufferDesc.ByteWidth = vertices.size() * sizeof( vertex_t );
	// data resource
	D3D11_SUBRESOURCE_DATA vdata;
	vdata.pSysMem = &vertices[0];
	// create vertex buffer on device using descriptor & data
	HRESULT vhr = Device->CreateBuffer( &vbufferDesc, &vdata, &VertexBuffer );

	//  index array descriptor
	D3D11_BUFFER_DESC ibufferDesc = { 0.0f };
	ibufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibufferDesc.CPUAccessFlags = 0;
	ibufferDesc.Usage = D3D11_USAGE_DEFAULT;
	ibufferDesc.MiscFlags = 0;
	ibufferDesc.ByteWidth = indices.size() * sizeof( unsigned );
	// data resource
	D3D11_SUBRESOURCE_DATA idata;
	idata.pSysMem = &indices[0];
	// create index buffer on device using descriptor & data
	HRESULT ihr = Device->CreateBuffer( &ibufferDesc, &idata, &IndexBuffer );

	// local data is now loaded to device so it can be released
	vertices.clear();
	nbr_indices = indices.size();
	indices.clear();
}

void CubeMesh::Render( ShaderProgram& shader ) const {
	//set topology
	DeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	// bind our vertex buffer
	UINT32 stride = sizeof( vertex_t ); //  sizeof(float) * 8;
	UINT32 offset = 0;
	DeviceContext->IASetVertexBuffers( 0, 1, &VertexBuffer, &stride, &offset );

	// bind our index buffer
	DeviceContext->IASetIndexBuffer( IndexBuffer, DXGI_FORMAT_R32_UINT, 0 );

	// make the drawcall
	DeviceContext->DrawIndexed( nbr_indices, 0, 0 );
}
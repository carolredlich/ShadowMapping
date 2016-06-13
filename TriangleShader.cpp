/*
 * File:   TriangleShader.cpp
 * Author: caroline
 *
 * Created on 8 de Abril de 2016, 10:53
 */

#include <cstring>
#include <cmath>
#include <string>
#include <cstdio>
#include <fstream>
#include <iostream>

#include "TriangleShader.h"

TriangleShader::TriangleShader( std::string vertexShaderFileName,
    std::string fragmentShaderFileName )
{
    std::string vertexShader = readFile( vertexShaderFileName.c_str( ) );
    std::string tesseletionControlShader = readFile( "triangle.tcs" );
    std::string tesseletionEvaluationShader = readFile( "triangle.tes" );
    std::string geometryShader = readFile( "triangle.geom" );
    std::string fragmentShader = readFile( fragmentShaderFileName.c_str( ) );


    std::cout << "Vertex Shader" << std::endl;
    std::cout << vertexShader << std::endl << std::endl << std::endl;
    std::cout << "Tesseletion Control Shader" << std::endl;
    std::cout << tesseletionControlShader << std::endl << std::endl << std::endl;
    std::cout << "Tesseletion Evaluation Shader" << std::endl;
    std::cout << tesseletionEvaluationShader << std::endl << std::endl << std::endl;
    std::cout << "Geometry Shader" << std::endl;
    std::cout << geometryShader << std::endl << std::endl << std::endl;
    std::cout << "Fragment Shader" << std::endl;
    std::cout << fragmentShader << std::endl << std::endl << std::endl;

    //Define os programas.
    if( vertexShader.size( ) > 0 )
    {
        setVertexProgram( vertexShader.c_str( ), vertexShader.size( ) );
    }
    if( tesseletionControlShader.size( ) > 0 )
    {
        setTesselationControlProgram( tesseletionControlShader.c_str( ), tesseletionControlShader.size( ) );
    }
    if( tesseletionEvaluationShader.size( ) > 0 )
    {
        setTesselationEvaluationProgram( tesseletionEvaluationShader.c_str( ), tesseletionEvaluationShader.size( ) );
    }
    if( geometryShader.size( ) > 0 )
    {
        setGeometryProgram( geometryShader.c_str( ), geometryShader.size( ) );
    }
    if( fragmentShader.size( ) > 0 )
    {
        setFragmentProgram( fragmentShader.c_str( ), fragmentShader.size( ) );
    }

    //Define os atributos para o geomtry shader.
    setGeometryParameters( GL_TRIANGLES, GL_TRIANGLE_STRIP, 3 );
}

TriangleShader::TriangleShader( const TriangleShader& orig ) { }

TriangleShader::~TriangleShader( ) { }

void TriangleShader::setMvpMatrix( double* mvp )
{
    for( int i = 0; i < 16; i++ )
    {
        _mvpMatrix[ i ] = mvp[ i ];
    }
}

void TriangleShader::setMMatrix( double* m )
{
    unsigned int j = 0;
    for( int i = 0; i < 16; i++ )
    {
        _mMatrix[ i ] = m[ i ];
    }
}

void TriangleShader::setVertices( double* vertices, int n )
{
    _vertex = vertices;
    _nVertices = n;
}

void TriangleShader::setNormal( double* normal )
{
    _normal = normal;
}

void TriangleShader::setMaterial( float* materialDifuse,
    float* materialSpecular, float* materialAmbient, float materialShiness )
{
    memcpy( _materialDifuse, materialDifuse, 3 * sizeof ( float ) );
    memcpy( _materialSpecular, materialSpecular, 3 * sizeof ( float ) );
    memcpy( _materialAmbient, materialAmbient, 3 * sizeof ( float ) );
    _materialShiness = materialShiness;
}

void TriangleShader::setTexCoord( double* texCoord )
{
    _texCoords = texCoord;
}

void TriangleShader::setTangentAndBitangent( double* tangent, double* bitangent )
{
    _tangent = tangent;
    _bitangent = bitangent;
}

void TriangleShader::loadVariables( )
{
    if( !isAllocated( ) )
    {
        return;
    }

    unsigned int parameterVertex = glGetAttribLocation( _glShader, "vertex_MS" );
    glBindBuffer( GL_ARRAY_BUFFER, parameterVertex );
    glVertexAttribPointer( parameterVertex, 3, GL_DOUBLE, GL_FALSE, 0, _vertex );
    glEnableVertexAttribArray( parameterVertex );

    unsigned int parameterNormal = glGetAttribLocation( _glShader, "normal_MS" );
    glVertexAttribPointer( parameterNormal, 3, GL_DOUBLE, GL_FALSE, 0, _normal );
    glEnableVertexAttribArray( parameterNormal );

    unsigned int parameterTexCoord = glGetAttribLocation( _glShader, "texCoord_MS" );
    glVertexAttribPointer( parameterTexCoord, 2, GL_DOUBLE, GL_FALSE, 0, _texCoords );
    glEnableVertexAttribArray( parameterTexCoord );

    unsigned int parameterTangent = glGetAttribLocation( _glShader, "tangent_MS" );
    glVertexAttribPointer( parameterTangent, 3, GL_DOUBLE, GL_FALSE, 0, _tangent );
    glEnableVertexAttribArray( parameterTangent );

    unsigned int parameterBitangent = glGetAttribLocation( _glShader, "bitangent_MS" );
    glVertexAttribPointer( parameterBitangent, 3, GL_DOUBLE, GL_FALSE, 0, _bitangent );
    glEnableVertexAttribArray( parameterBitangent );


    //Textura
    unsigned int parameterTexture1 = glGetUniformLocation( _glShader, "colorTextureSampler" );
    glUniform1i( parameterTexture1, 0 );

    unsigned int parameterTexture2 = glGetUniformLocation( _glShader, "normalTextureSampler" );
    glUniform1i( parameterTexture2, 1 );

    //Material
    unsigned int parameterMaterialDifuse = glGetUniformLocation( _glShader, "materialDifuse" );
    glUniform3f( parameterMaterialDifuse, _materialDifuse[ 0 ], _materialDifuse[ 1 ], _materialDifuse[ 2 ] );

    unsigned int parameterMaterialAmbient = glGetUniformLocation( _glShader, "materialAmbient" );
    glUniform3f( parameterMaterialAmbient, _materialAmbient[ 0 ], _materialAmbient[ 1 ], _materialAmbient[ 2 ] );

    unsigned int parameterMaterialSpecular = glGetUniformLocation( _glShader, "materialSpecular" );
    glUniform3f( parameterMaterialSpecular, _materialSpecular[ 0 ], _materialSpecular[ 1 ], _materialSpecular[ 2 ] );
    
    unsigned int parameterMaterialShiness = glGetUniformLocation( _glShader, "materialShiness" );
    glUniform1f( parameterMaterialShiness,  _materialShiness );

    //Matrizes
    unsigned int parameterMvp = glGetUniformLocation( _glShader, "mvp" );
    glUniformMatrix4fv( parameterMvp, 1, GL_FALSE, _mvpMatrix );

    unsigned int parameterM = glGetUniformLocation( _glShader, "m" );
    glUniformMatrix4fv( parameterM, 1, GL_FALSE, _mMatrix );
}

std::string TriangleShader::readFile( const char* name )
{
    std::ifstream in( name );
    std::string shader;

    if( in.fail( ) )
    {
        return "";
    }

    char a;
    while( in.get( a ) && a != EOF )
    {
        shader += a;
    }
    shader += '\0';
    return shader;
}



const canvas = document.querySelector("#fig");
const gl = canvas.getContext("webgl");
if (gl === null) {
	console.error("Unable to initialize WebGL. Your browser or machine may not support it.");
}

const vsSource = `
attribute vec4 aVertexPosition;
attribute vec4 aVertexColor;
varying vec4 color;

uniform mat4 uModelViewMatrix;
uniform mat4 uProjectionMatrix;

void main() {
	gl_Position = uProjectionMatrix * uModelViewMatrix * aVertexPosition;
	color = vec4(gl_Position.x, gl_Position.y, 0.0, 1.0);
}
`;

const fsSource = `
varying mediump vec4 color;
void main() {
	gl_FragColor = color;
}
`;

const shaderProgram = initShaderProgram(gl, vsSource, fsSource);
const programInfo = {
	program: shaderProgram,
	attribLocations: {
		vertexPosition: gl.getAttribLocation(shaderProgram, 'aVertexPosition'),
		vertexColor: gl.getAttribLocation(shaderProgram, 'aVertexColor'),
	},
	uniformLocations: {
		projectionMatrix: gl.getUniformLocation(shaderProgram, 'uProjectionMatrix'),
		modelViewMatrix: gl.getUniformLocation(shaderProgram, 'uModelViewMatrix'),
	},
};

const buffers = initBuffers(gl);
drawScene(gl, programInfo, buffers);

function initShaderProgram(gl, vsSource, fsSource) {
	const vertexShader = loadShader(gl, gl.VERTEX_SHADER, vsSource);
	const fragmentShader = loadShader(gl, gl.FRAGMENT_SHADER, fsSource);

	// Create the shader program

	const shaderProgram = gl.createProgram();
	gl.attachShader(shaderProgram, vertexShader);
	gl.attachShader(shaderProgram, fragmentShader);
	gl.linkProgram(shaderProgram);

	// If creating the shader program failed, alert

	if (!gl.getProgramParameter(shaderProgram, gl.LINK_STATUS)) {
		console.error('Unable to initialize the shader program: ' + gl.getProgramInfoLog(shaderProgram));
		return null;
	}

	return shaderProgram;
}

function loadShader(gl, type, source) {
	const shader = gl.createShader(type);
	gl.shaderSource(shader, source);
	gl.compileShader(shader);

	if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
		console.error('An error occurred compiling the shaders: ' + gl.getShaderInfoLog(shader));
		gl.deleteShader(shader);
		return null;
	}

	return shader;
}

function initBuffers(gl) {
	const positionBuffer = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER, positionBuffer);

	gl.bufferData(
		gl.ARRAY_BUFFER,
		new Float32Array([
			-1.0,  1.0,
			 1.0,  1.0,
			-1.0, -1.0,
			 1.0, -1.0,
		]),
		gl.STATIC_DRAW);
	
	const colorBuffer = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER, colorBuffer)
	gl.bufferData(
		gl.ARRAY_BUFFER,
		new Float32Array([
			
		]),
		gl.STATIC_DRAW)

	return {
		position: positionBuffer,
		colors: null,
	};
}

function drawScene(gl, programInfo, buffers) {
	gl.clearColor(0.0, 0.0, 0.0, 1.0);  // Clear to black, fully opaque
	gl.clearDepth(1.0);                 // Clear everything
	gl.enable(gl.DEPTH_TEST);           // Enable depth testing
	gl.depthFunc(gl.LEQUAL);            // Near things obscure far things

	gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

	{
		const numComponents = 2;  // pull out 2 values per iteration
		const type = gl.FLOAT;    // the data in the buffer is 32bit floats
		const normalize = false;  // don't normalize
		const stride = 0;         // how many bytes to get from one set of values to the next
															// 0 = use type and numComponents above
		const offset = 0;         // how many bytes inside the buffer to start from
		gl.bindBuffer(gl.ARRAY_BUFFER, buffers.position);
		gl.vertexAttribPointer(
				programInfo.attribLocations.vertexPosition,
				numComponents,
				type,
				normalize,
				stride,
				offset);
		gl.enableVertexAttribArray(
				programInfo.attribLocations.vertexPosition);
	}
	{
		const numComponents = 2;  // pull out 2 values per iteration
		const type = gl.FLOAT;    // the data in the buffer is 32bit floats
		const normalize = false;  // don't normalize
		const stride = 0;         // how many bytes to get from one set of values to the next
															// 0 = use type and numComponents above
		const offset = 0;         // how many bytes inside the buffer to start from
		gl.bindBuffer(gl.ARRAY_BUFFER, buffers.position);
		gl.vertexAttribPointer(
				programInfo.attribLocations.colorPosition,
				numComponents,
				type,
				normalize,
				stride,
				offset);
		gl.enableVertexAttribArray(
				programInfo.attribLocations.colorPosition);
	}

	gl.useProgram(programInfo.program);
	gl.uniformMatrix4fv(
			programInfo.uniformLocations.projectionMatrix,
			false,
			new Float32Array([
				0.5, 0.0, 0.0, 0.0,
				0.0, 0.5, 0.0, 0.0,
				0.0, 0.0, 0.5, 0.0,
				0.0, 0.0, 0.0, 1.0
			]));
	gl.uniformMatrix4fv(
			programInfo.uniformLocations.modelViewMatrix,
			false,
			new Float32Array([
				1.0, 0.0, 0.0, 0.0,
				0.0, 1.0, 0.0, 0.0,
				0.0, 0.0, 1.0, -6.0,
				0.0, 0.0, 0.0, 1.0
			]));

	{
		const offset = 0;
		const vertexCount = 4;
		gl.drawArrays(gl.TRIANGLE_STRIP, offset, vertexCount);
	}
}

#version 450 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in ivec4 BoneIDs;
layout(location = 4) in vec4 Weights;


layout(std430, binding = 0) buffer ssbo1  //for specifying joints : full
 {
	mat4 aInstanceMatrix[];
 };


 
out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoord;
out vec3 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 viewProjection;

uniform mat3 normalMatrix;


uniform mat4 gBones[150];
uniform mat2x4 dqs[150];


//uniform bool lbsOn;
//uniform bool dqsOn;
//uniform float ratio;

bool lbsOn = false;
bool dqsOn = true;
float ratio = 0.0f;


mat4x4 DQtoMat(vec4 real, vec4 dual) {
	mat4x4 m;
	float len2 = dot(real, real);
	float w = real.w, x = real.x, y = real.y, z = real.z;
	float t0 = dual.w, t1 = dual.x, t2 = dual.y, t3 = dual.z;

	m[0][0] = w * w + x * x - y * y - z * z;
	m[1][0] = 2 * x * y - 2 * w * z;
	m[2][0] = 2 * x * z + 2 * w * y;
	m[0][1] = 2 * x * y + 2 * w * z;
	m[1][1] = w * w + y * y - x * x - z * z;
	m[2][1] = 2 * y * z - 2 * w * x;
	m[0][2] = 2 * x * z - 2 * w * y;
	m[1][2] = 2 * y * z + 2 * w * x;
	m[2][2] = w * w + z * z - x * x - y * y;

	m[3][0] = -2 * t0 * x + 2 * w * t1 - 2 * t2 * z + 2 * y * t3;
	m[3][1] = -2 * t0 * y + 2 * t1 * z - 2 * x * t3 + 2 * w * t2;
	m[3][2] = -2 * t0 * z + 2 * x * t2 + 2 * w * t3 - 2 * t1 * y;

	m[0][3] = 0;
	m[1][3] = 0;
	m[2][3] = 0;
	m[3][3] = len2;
	m /= len2;

	return m;
}



mat4 rotationMatrix(vec3 axis, float angle)
{
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;
    
    return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
                0.0,                                0.0,                                0.0,                                1.0);
}


/*
// @brief procedure to fix the bulge artifact of Dual Quaternion
struct Dqs_fix {
    // A bone is defined as a segment (origin, direction and length in 3D space)
    std::vector<anim::Bone> _bone_rest_pose;
    // _weights[vertex Index] == map<bone_index, bone_skinning_weight>
    std::vector< std::map<int, float> > _weights;
 
    /// Bulging-free dual quaternion skinning
    /// "YoungBeom Kim andJungHyun Han"
    inline Vec3 apply(
            int id, // Index of the vertex we correct
            const Point3& rest_vert, // Vertex position at bind pose / T pose
            const Point3& dq_in, // Vertex after dual quaternion skinning
            const std::vector<Mat4x4> skinning_transfo) // Global transformation for each joint
    {
        // find the index of the bone with the highest weight for vertex id
        int major_bone = anim::find_max_index(_weights[id]);
 
        // Find out the distance to the bone in rest pose and after skinning.
        const anim::Bone& bone_rest = _bone_rest_pose[major_bone];
        float dv_rest = bone_rest.dist_from_segment(rest_vert);
 
        const anim::Bone bone_current = bone_rest.transform(skinning_transfo[major_bone]);
        float dv_current = bone_current.dist_from_segment(dq_in);
 
        Vec3 result = dq_in;
 
        // If dv has grown then bulging is detected so we draw the vertex towards
        // the closest bone point.
        if (dv_current > dv_rest)
        {
            Point3 proj_point = bone_current.project_on_segment(dq_in);
            Vec3 proj_dir  = dq_in - proj_point;
            const float  factor = dv_rest / dv_current;
            result = proj_point + (factor * proj_dir);
        }
        return result;
    }
 
};
 
// -------------------------------------------------------------------------
 
/// Finds the max index in a weight map.
int find_max_index( const std::map<int, float> >& weights)
{
   auto it = weights.begin();
   auto maxit = weights.begin();
   float max = 0.f;
   for( ;  it != weights.end(); ++it) {
        if (it->second > max)
        {
            max = it->second;
            maxit = it;
        }
    }
    assert(maxit->second == max);
    return maxit->first;
}
 
// -------------------------------------------------------------------------
 
float Bone::dist_from_segment(const Point3& p) const {
    Vec3 op = p - _org;
    float x = op.dot(_dir) / (_length * _length);
    x = fminf(1.f, fmaxf(0.f, x));
    Point3 proj = _org + _dir * x;
    float d = proj.distance_squared(p);
    return sqrtf(d);
}
 
// -------------------------------------------------------------------------
Point3 Bone::project_on_segment(const Point3& p) const
{
    const Vec3 op = p - _org;
    float d = op.dot(_dir.normalized()); // projected dist from origin
 
    if(d < 0)            return _org;
    else if(d > _length) return _org + _dir;
    else                 return _org + _dir.normalized() * d;
}

*/


#define MAX_COLORS 10
vec3 meshletcolors[MAX_COLORS] = {
  vec3(0.7,0,0), 
  vec3(0,0.6,0),
  vec3(0,0,0.8),
  vec3(0.7,0.7,0),
  vec3(0.7,0,0.5),
  vec3(0,0.6,0.8),
  vec3(0.9,0.5,0),
  vec3(0.5,0.8,0),
  vec3(0,0.5,0.9),
  vec3(0.8,0.8,0.8)
};



void main() {


	TexCoord = aTexCoords;
	mat4 BoneTransform = mat4(1.0);
	
	BoneTransform = gBones[BoneIDs[0]] * Weights[0];

	//if (BoneTransform == 0.0f) {
	//	BoneTransform = mat4(1.0);
	//}


	BoneTransform += gBones[BoneIDs[1]] * Weights[1];
	BoneTransform += gBones[BoneIDs[2]] * Weights[2];
	BoneTransform += gBones[BoneIDs[3]] * Weights[3];


	mat2x4 dq0 = dqs[BoneIDs[0]];
	mat2x4 dq1 = dqs[BoneIDs[1]];
	mat2x4 dq2 = dqs[BoneIDs[2]];
	mat2x4 dq3 = dqs[BoneIDs[3]];

	if (dot(dq0[0], dq1[0]) < 0.0) dq1 *= -1.0;
	if (dot(dq0[0], dq2[0]) < 0.0) dq2 *= -1.0;
	if (dot(dq0[0], dq3[0]) < 0.0) dq3 *= -1.0;

	mat2x4 blendDQ = dq0 * Weights[0];
	blendDQ += dq1 * Weights[1];
	blendDQ += dq2 * Weights[2];
	blendDQ += dq3 * Weights[3];



	mat4 DQmat = DQtoMat(blendDQ[0], blendDQ[1]);



	mat4 model2  = aInstanceMatrix[gl_InstanceID];

	color = meshletcolors[gl_InstanceID % MAX_COLORS];

//	vec4 trans = model2[3];
//	trans.x = trans.x;
//	trans.z = trans.z;
//	model2[3] = trans;

	if (lbsOn) {
		vec4 pos = BoneTransform * vec4(aPos, 1.0);
		gl_Position = projection * view * model * pos;
		FragPos = vec3(model* pos);
		Normal = mat3(transpose(inverse(BoneTransform))) * aNormal;
	//	Normal = aNormal;
	}
	else if (dqsOn)
	{
		
		float len = length(blendDQ[0]);
		blendDQ /= len;

		vec3 position = aPos.xyz + 2.0*cross(blendDQ[0].xyz, cross(blendDQ[0].xyz, aPos.xyz) + blendDQ[0].w * aPos.xyz);
		vec3 trans = 2.0*(blendDQ[0].w * blendDQ[1].xyz - blendDQ[1].w * blendDQ[0].xyz + cross(blendDQ[0].xyz, blendDQ[1].xyz));
		position += trans;

		gl_Position = viewProjection * model2 * vec4(position, 1.0f);
	//	gl_Position = projection * view * aInstanceMatrix * vec4(position, 1.0f);


		//vec4 pos = DQmat * vec4(aPos, 1.0);
		//gl_Position = projection * view * model * pos;
		FragPos = vec3(model2* vec4(position, 1.0f));
		//FragPos = vec3(aInstanceMatrix* vec4(position, 1.0f));

		Normal = mat3(transpose(inverse(DQmat))) * aNormal;
		//Normal = mat3(transpose(inverse(model2))) * aNormal;
		//Normal = normalMatrix * aNormal;

	}
	else {
		mat4 Mix = (1 - ratio) * BoneTransform + ratio * DQmat;
		vec4 pos = Mix * vec4(aPos, 1.0);
		gl_Position = projection * view * model * pos;
		FragPos = vec3(model* pos);
		Normal = mat3(transpose(inverse(model))) * aNormal;
//		Normal = aNormal;
	}

	


	

}
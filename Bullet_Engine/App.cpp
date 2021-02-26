#include "App.h"

#define RADIANS_PER_DEGREE 0.01745329f
#define CAMERA_STEP_SIZE 5.0f

App::App() :

	m_cameraPosition(10.0f, 5.0f, 0.0f),
	m_cameraTarget(0.0f, 0.0f, 0.0f),
	m_cameraDistance(15.0f),
	m_cameraPitch(20.0f),
	m_cameraYaw(0.0f),
	m_upVector(0.0f, 1.0f, 0.0f),
	m_nearPlane(1.0f),
	m_farPlane(1000.0f),
	m_pBroadphase(0),
	m_pCollisionConfiguration(0),
	m_pDispatcher(0),
	m_pSolver(0),
	m_pWorld(0)
{
}

App::~App()
{
	ShutdownPysics();
}

void App::init()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClearColor(0.6f, 0.65f, 0.85f, 0);

	InitPhysics();
}

void App::Keyboard(unsigned char key, int x, int y)
{
}

void App::KeyboardUp(unsigned char key, int x, int y)
{
}

void App::Special(int key, int x, int y)
{
}

void App::SpeciaUp(int key, int x, int y)
{
}

void App::Reshape(int w, int h)
{
	m_screenWidth = w;
	m_screenHeight = h;

	glViewport(0, 0, w, h);
	UpadateCamera();
}

void App::Idle()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	float dt = m_clock.getTimeMilliseconds();
	m_clock.reset();
	UpdateScene(dt / 1000);
	UpadateCamera();
	RenderScene();

	glutSwapBuffers();
}

void App::Mouse(int button, int state, int x, int y)
{
}

void App::PassiveMotion(int x, int y)
{
}

void App::Motion(int x, int y)
{
}

void App::Display()
{
}

void App::RenderScene()
{
	btScalar transform[16];
	//if (m_pMotionState)
	//{
	//	m_pMotionState->GetWorldTransform(transform);

	//	glPushMatrix();
	//	glMultMatrixf(transform);
	//	glutSolidCube(1);
	//	glpopMatrix();
	//}

	for (GameObjects::iterator i = m_objects.begin(); i != m_objects.end(); i++)
	{
		GameObject* pObj = *i;

		pObj->GetTransform(transform);
		DrawShape(transform, pObj->GetShape(), pObj->GetColor());
	}
}

void App::UpdateScene(float dt)
{
	if (m_pWorld)
	{
		m_pWorld->stepSimulation(dt);
	}
}

void App::UpadateCamera()
{
	if (m_screenWidth == 0 && m_screenHeight == 0)
		return;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float aspectRatio = (float)m_screenWidth / (float)m_screenHeight;
	glFrustum(-aspectRatio * m_nearPlane, aspectRatio * m_nearPlane, -m_nearPlane, m_nearPlane, m_nearPlane, m_farPlane);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	float pitch = m_cameraPitch * RADIANS_PER_DEGREE;
	float yaw = m_cameraYaw * RADIANS_PER_DEGREE;


	btQuaternion rotation(m_upVector, yaw);
	// set the camera's position to 0,0,0, then move the 'z' 
// position to the current value of m_cameraDistance.
	btVector3 cameraPosition(0, 0, 0);
	cameraPosition[2] = -m_cameraDistance;

	// create a Bullet Vector3 to represent the camera 
	// position and scale it up if its value is too small.
	btVector3 forward(cameraPosition[0], cameraPosition[1], cameraPosition[2]);
	if (forward.length2() < SIMD_EPSILON) {
		forward.setValue(1.f, 0.f, 0.f);
	}

	// figure out the 'right' vector by using the cross 
	// product on the 'forward' and 'up' vectors
	btVector3 right = m_upVector.cross(forward);

	// create a quaternion that represents the camera's roll
	btQuaternion roll(right, -pitch);
	
	cameraPosition = btMatrix3x3(rotation) * btMatrix3x3(roll) * cameraPosition;

	// save our new position in the member variable, and 
	// shift it relative to the target position (so that we 
	// orbit it)
	m_cameraPosition[0] = cameraPosition.getX();
	m_cameraPosition[1] = cameraPosition.getY();
	m_cameraPosition[2] = cameraPosition.getZ();
	m_cameraPosition += m_cameraTarget;

	gluLookAt(m_cameraPosition[0], m_cameraPosition[1], m_cameraPosition[2],
		m_cameraTarget[0], m_cameraTarget[1], m_cameraTarget[2],
		m_upVector.getX(), m_upVector.getY(), m_upVector.getZ());
}

void App::DrawBox(const btVector3 & halfSize) {

	/*REM*	// push the transform onto the stack **/
	/*REM*	glPushMatrix(); **/
	/*REM*	glMultMatrixf(transform); **/

	float halfWidth = halfSize.x();
	float halfHeight = halfSize.y();
	float halfDepth = halfSize.z();

	/*REM*	// set the object's color **/
	/*REM*	glColor3f(color.x(), color.y(), color.z()); **/

		// create the vertex positions
	btVector3 vertices[8] = {
	btVector3(halfWidth,halfHeight,halfDepth),
	btVector3(-halfWidth,halfHeight,halfDepth),
	btVector3(halfWidth,-halfHeight,halfDepth),
	btVector3(-halfWidth,-halfHeight,halfDepth),
	btVector3(halfWidth,halfHeight,-halfDepth),
	btVector3(-halfWidth,halfHeight,-halfDepth),
	btVector3(halfWidth,-halfHeight,-halfDepth),
	btVector3(-halfWidth,-halfHeight,-halfDepth) };

	// create the indexes for each triangle, using the 
	// vertices above. Make it static so we don't waste 
	// processing time recreating it over and over again
	static int indices[36] = {
		0,1,2,
		3,2,1,
		4,0,6,
		6,0,2,
		5,1,4,
		4,1,0,
		7,3,1,
		7,1,5,
		5,4,7,
		7,4,6,
		7,2,3,
		7,6,2 };

	// start processing vertices as triangles
	glBegin(GL_TRIANGLES);

	// increment the loop by 3 each time since we create a 
	// triangle with 3 vertices at a time.

	for (int i = 0; i < 36; i += 3) {
		// get the three vertices for the triangle based
		// on the index values set above
		// use const references so we don't copy the object
		// (a good rule of thumb is to never allocate/deallocate
		// memory during *every* render/update call. This should 
		// only happen sporadically)
		const btVector3& vert1 = vertices[indices[i]];
		const btVector3& vert2 = vertices[indices[i + 1]];
		const btVector3& vert3 = vertices[indices[i + 2]];

		// create a normal that is perpendicular to the 
		// face (use the cross product)
		btVector3 normal = (vert3 - vert1).cross(vert2 - vert1);
		normal.normalize();

		// set the normal for the subsequent vertices
		glNormal3f(normal.getX(), normal.getY(), normal.getZ());

		// create the vertices
		glVertex3f(vert1.x(), vert1.y(), vert1.z());
		glVertex3f(vert2.x(), vert2.y(), vert2.z());
		glVertex3f(vert3.x(), vert3.y(), vert3.z());
	}

	// stop processing vertices
	glEnd();

	/*REM*	// pop the transform from the stack in preparation **/
	/*REM*	// for the next object **/
	/*REM*	glPopMatrix(); **/
}

void App::DrawShape(btScalar* transform, const btCollisionShape* pShape, const btVector3& color)
{
	glColor3f(color[0], color[1], color[2]);

	glPushMatrix();
	glMultMatrixf(transform);

	switch (pShape->getShapeType())
	{
	case BOX_SHAPE_PROXYTYPE:
	{
		const btBoxShape* box = static_cast<const btBoxShape*>(pShape);
		btVector3 halfSize = box->getHalfExtentsWithMargin();
		DrawBox(halfSize);
		//glutSolidCube(5);
		break;
	}
	default:
	{
		break;
	}
	}
	glPopMatrix();
}

GameObject* App::CreateGameObject(btCollisionShape* pShape, const float& mass, const btVector3& color, const btVector3& initPosition, const btQuaternion& initRotaion)
{
	GameObject* pObject = new GameObject(pShape, mass, color, initPosition, initRotaion);
	m_objects.push_back(pObject);

	if (m_pWorld)
	{
		m_pWorld->addRigidBody(pObject->GetRigidBody());
	}
	return pObject;
}

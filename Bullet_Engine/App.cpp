#include "App.h"
#include "bullet/CommonInterfaces/CommonRigidBodyBase.h"

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
	m_pWorld(0),
	DeltaTime(0)
{
}

App::~App()
{
	ShutdownPhysics();
}

void App::init()
{
	GLfloat ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat position[] = { 5.0f, 10.0f, 1.0f, 1.0f };

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);

	glShadeModel(GL_SMOOTH);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glClearColor(0.6f, 0.65f, 0.85f, 0);
	InitPhysics();

	m_pDebugDrawer = new DebugDrawer();
	m_pDebugDrawer->setDebugMode(0);
	m_pWorld->setDebugDrawer(m_pDebugDrawer);

	//==========================
	//Custom
	//==========================
	//Player = FindGameObject("Player");

	//==========================
	//==========================

}

void App::Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'z':
		m_pDebugDrawer->ToggleDebugFlag(btIDebugDraw::DBG_DrawWireframe);
		break;
	case 'x':
		m_pDebugDrawer->ToggleDebugFlag(btIDebugDraw::DBG_DrawAabb);
		break;

	//======================================================
	case 't': {
		RayResult result;
		if (!Raycast(m_cameraPosition, GetPickingRay(x, y), result))
			return;
		if (result.pBody == Player->GetRigidBody()) 
			return;
		DestroyGameObject(result.pBody);
		break;
	}
	
	case 27://ESC key
		exit(0);
		break;
	default:
		break;
	}
}

void App::KeyboardUp(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'r':
		GameObjectReset();
		break;
	case ' ': {
		break;
	}
	default:
		break;
	}
	
}

void App::Special(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		RotateCamera(m_cameraYaw, +CAMERA_STEP_SIZE, false);
		break;
	case GLUT_KEY_RIGHT:
		RotateCamera(m_cameraYaw, -CAMERA_STEP_SIZE, false);
		break;
	case GLUT_KEY_UP:
		RotateCamera(m_cameraPitch, +CAMERA_STEP_SIZE, true);
		break;
	case GLUT_KEY_DOWN:
		RotateCamera(m_cameraPitch,-CAMERA_STEP_SIZE, true);
		break;
	default:
		break;
	}
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
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//UpadateCamera();

	//float dt = m_clock.getTimeMilliseconds();
	//m_clock.reset();
	//DeltaTime = dt / 1000.0f;
	//UpdateScene(DeltaTime);

	////DrawBox(btVector3(5, 5, 5));
	//RenderScene(DeltaTime);
	//glutSwapBuffers();

	glutPostRedisplay();
}

void App::Mouse(int button, int state, int x, int y)
{
	switch (button)
	{
	case GLUT_LEFT_BUTTON: {

		if (state == 0)
		{
			RayResult result;
			if (!Raycast(m_cameraPosition, GetPickingRay(x, y), result)) return;
			PlayerSelection(result.pBody);
		}
		break;
	}

	case GLUT_RIGHT_BUTTON: {
		
		if (state == 0)
		{
			ShootBox(GetPickingRay(x, y));
		}
		break;
	}
	default:
		break;
	}
	//glutPostRedisplay();
}

void App::MouseWheel(int button, int dir, int x, int y)
{
	if (dir > 0)
	{
		ZoomCamera(+CAMERA_STEP_SIZE);
	}
	else
	{
		ZoomCamera(-CAMERA_STEP_SIZE);
	}
}

void App::PassiveMotion(int x, int y)
{
}

void App::Motion(int x, int y)
{
}

void App::Display()
{ 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	UpadateCamera();

	float dt = m_clock.getTimeMilliseconds();
	m_clock.reset();
	DeltaTime = dt / 1000.0f;
	UpdateScene(DeltaTime);

	//DrawBox(btVector3(5, 5, 5));
	RenderScene(DeltaTime);
	glutSwapBuffers();
}

void App::RenderScene(float dt)
{
	btScalar transform[16];
	//if (m_pWorld)
	//{
	//	m_pMotionState->GetWorldTransform(transform);
	//	DrawBox(transform, btVector3(1,1,1), btVector3(1, 0, 0));
	//}
	for (GameObjects::iterator i = m_objects.begin(); i != m_objects.end(); ++i)
	{
		GameObject* pObj = *i;
		pObj->GetTransform(transform);
		//std::cout << pObj->GetRigidBody()->getMass()<< std::endl;
		DrawShape(transform, pObj->GetShape(), pObj->GetColor());
		
	}
	m_pWorld->debugDrawWorld();	
}

void App::DrawShape(btScalar* transform, const btCollisionShape* pShape, const btVector3& color)
{
	glColor3f(color.x(), color.y(), color.z());

	glPushMatrix();
	glMultMatrixf(transform);

	switch (pShape->getShapeType())
	{
	case BOX_SHAPE_PROXYTYPE:
	{
		const btBoxShape* box = static_cast<const btBoxShape*>(pShape);
		btVector3 halfSize = box->getHalfExtentsWithMargin();
		DrawBox(halfSize);
		break;
	}
	case SPHERE_SHAPE_PROXYTYPE:
	{
		const btSphereShape* sphere = static_cast<const btSphereShape*>(pShape);
		float radius = sphere->getMargin();
		DrawSphere(radius);
		break;
	}


	default:
		break;
	}
	glPopMatrix();
}

void App::UpdateScene(float dt)
{
	if (m_pWorld)
	{
		m_pWorld->stepSimulation(dt);
	}
	CheckForCollisionEvents();
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

	btVector3 cameraPosition(0, 0, 0);
	cameraPosition[2] = -m_cameraDistance;

	btVector3 forward(cameraPosition);
	if (forward.length2() < SIMD_EPSILON)
	{
		forward.setValue(1.0f, 0.0f, 0.0f);
	}
	btVector3 right = m_upVector.cross(forward);
	btQuaternion roll(right, -pitch);
	cameraPosition = btMatrix3x3(rotation) * btMatrix3x3(roll) * cameraPosition;

	m_cameraPosition[0] = cameraPosition.getX();
	m_cameraPosition[1] = cameraPosition.getY();
	m_cameraPosition[2] = cameraPosition.getZ();
	

	//=============================
	//Custom
	//=============================
	if (Player)
	{
		btTransform transform;
		Player->GetMotionState()->getWorldTransform(transform);

		m_cameraPosition[0] += transform.getOrigin().getX();
		m_cameraPosition[1] += transform.getOrigin().getY();
		m_cameraPosition[2] += transform.getOrigin().getZ();

		m_cameraTarget[0] = transform.getOrigin().getX();
		m_cameraTarget[1] = transform.getOrigin().getY();
		m_cameraTarget[2] = transform.getOrigin().getZ();
	}
	
	gluLookAt(m_cameraPosition[0], m_cameraPosition[1], m_cameraPosition[2],
		m_cameraTarget[0], m_cameraTarget[1], m_cameraTarget[2],
		m_upVector.getX(), m_upVector.getY(), m_upVector.getZ());


	btVector3 rayFrom = m_cameraPosition;
	btVector3 rayForward = (m_cameraTarget - m_cameraPosition);
	CameraFront = rayForward.normalize();
	//rayForward *= m_farPlane;

	btVector3 ver = m_upVector;
	btVector3 hor = rayForward.cross(ver);
	CameraRight = hor.normalize();
	CameraFront.setY(0);
	//==============================
}

//=================================
//Custom
void App::RotateCamera(float& angle, float value, bool Move_mode)
{
	angle += value;
	if (angle <= -90 && Move_mode) angle = -90;
	if (angle >= 90 && Move_mode) angle = 90;
	UpadateCamera();
}
//=================================

void App::ZoomCamera(float distance)
{
	m_cameraDistance -= distance;
	if (m_cameraDistance < 0.1f) m_cameraDistance = 0.1f;
	UpadateCamera();
}

void App::ShootBox(const btVector3& direction)
{
	GameObject* pObject = CreateGameObject(new btBoxShape(btVector3(1.0f, 1.0f, 1.0f)), 1.0f, btVector3(0, 1, 1), m_cameraPosition);
	pObject->SetTag("Bullet");

	btVector3 velocity = direction;
	velocity.normalize();
	velocity *= 25.0f;

	pObject->GetRigidBody()->setLinearVelocity(velocity);
}

void App::DestroyGameObject(btRigidBody* pBody)
{
	for (GameObjects::iterator iter = m_objects.begin(); iter != m_objects.end(); ++iter)
	{
		if ((*iter) -> GetRigidBody() == pBody)
		{
			GameObject* pObject = *iter;
			m_pWorld->removeRigidBody(pObject->GetRigidBody());
			m_objects.erase(iter);
			delete pObject;
			return;
		}
	}
}

void App::GameObjectReset()
{
	for (GameObjects::iterator iter = m_objects.begin(); iter != m_objects.end() - 1; ++iter)
	{
		GameObject* pObject = *iter;
		m_pWorld->removeRigidBody(pObject->GetRigidBody());
		delete pObject;
	}
	m_objects.clear();
	ShutdownPhysics();
	InitPhysics();
	m_pWorld->setDebugDrawer(m_pDebugDrawer);
}

void App::PlayerSelection(btRigidBody* pBody)
{
	for (GameObjects::iterator iter = m_objects.begin(); iter != m_objects.end(); ++iter)
	{
		if ((*iter)->GetRigidBody() == pBody)
		{
			Player = *iter;
			return;
		}
	}
}

GameObject* App::FindGameObject(btRigidBody* pBody)
{
	for (GameObjects::iterator iter = m_objects.begin(); iter != m_objects.end(); ++iter)
	{
		if ((*iter)->GetRigidBody() == pBody)
		{
			return *iter;
		}
	}
	return 0;
}

btVector3 App::GetPickingRay(int x, int y)
{
	float tanFov = 1.0f / m_nearPlane;
	float fov = btScalar(2.0) * btAtan(tanFov);

	btVector3 rayFrom = m_cameraPosition;
	btVector3 rayForward = (m_cameraTarget - m_cameraPosition);
	rayForward.normalize();
	rayForward *= m_farPlane;

	btVector3 ver = m_upVector;
	btVector3 hor = rayForward.cross(ver);
	hor.normalize();
	ver = hor.cross(rayForward);
	ver.normalize();
	hor *= 2.0f * m_farPlane * tanFov;
	ver *= 2.0f * m_farPlane * tanFov;

	btScalar aspect = m_screenWidth / (btScalar)m_screenHeight;
	hor *= aspect;
	btVector3 rayToCenter = rayFrom + rayForward;
	btVector3 dHor = hor * 1.0f / float(m_screenWidth);
	btVector3 dVert = ver * 1.0f / float(m_screenHeight);
	btVector3 rayTo = rayToCenter - 0.5f * hor + 0.5f * ver;
	rayTo += btScalar(x) * dHor;
	rayTo -= btScalar(y) * dVert;

	return rayTo;
}

bool App::Raycast(const btVector3& startPosition, const btVector3& direction, RayResult& output)
{
	if (!m_pWorld)
		return false;

	btVector3 rayTo = direction;
	btVector3 rayFrom = m_cameraPosition;

	btCollisionWorld::ClosestRayResultCallback rayCallback(rayFrom, rayTo);
	
	m_pWorld->rayTest(rayFrom, rayTo, rayCallback);
	if (rayCallback.hasHit())
	{
		btRigidBody* pBody = (btRigidBody*)btRigidBody::upcast(rayCallback.m_collisionObject);
		if (!pBody)
			return false;
		if (pBody->isStaticObject() || pBody->isKinematicObject())
			return false;

		output.pBody = pBody;
		output.hitPoint = rayCallback.m_hitPointWorld;
		return true;
	}

	return false;
}

void App::CheckForCollisionEvents()
{
	CollisionPairs pairsThisUpdate;
	
	for(int i = 0; i < m_pDispatcher->getNumManifolds(); ++i)
	{
		btPersistentManifold* pManifold = m_pDispatcher->getManifoldByIndexInternal(i);
		if (pManifold->getNumContacts() > 0)
		{
			const btRigidBody* pBody0 = static_cast<const btRigidBody*>(pManifold->getBody0());
			const btRigidBody* pBody1 = static_cast<const btRigidBody*>(pManifold->getBody1());
		
			bool const swapped = pBody0 > pBody1;
			const btRigidBody* pSortedBodyA = swapped ? pBody1 : pBody0;
			const btRigidBody* pSortedBodyB = swapped ? pBody0 : pBody1;

			CollisionPair thisPair = std::make_pair(pSortedBodyA, pSortedBodyB);
			pairsThisUpdate.insert(thisPair);

			if (m_pairsLastUpdate.find(thisPair) == m_pairsLastUpdate.end())
			{
				CollisionEvent((btRigidBody*)pBody0, (btRigidBody*)pBody1);
			}
		}
	}
	
	CollisionPairs removedPairs;

	std::set_difference(m_pairsLastUpdate.begin(), m_pairsLastUpdate.end(),
						pairsThisUpdate.begin(), pairsThisUpdate.end(),
						std::inserter(removedPairs, removedPairs.end()));

	for (CollisionPairs::const_iterator iter = removedPairs.begin(); iter != removedPairs.end(); ++iter)
	{
		SeparationEvent((btRigidBody*)iter->first, (btRigidBody*)iter->second);
	}

	m_pairsLastUpdate = pairsThisUpdate;
}

void App::CollisionEvent(btRigidBody* pBody0, btRigidBody* pBody1)
{
	//GameObject* pObj0 = FindGameObject(pBody0);
	//GameObject* pObj1 = FindGameObject(pBody1);

	//if (!pObj0 || !pObj1) return;
	//
	//pObj0->SetColor(btVector3(1.0, 1.0, 1.0));
	//pObj1->SetColor(btVector3(1.0, 1.0, 1.0));
}

void App::SeparationEvent(btRigidBody* pBody0, btRigidBody* pBody1)
{
	//GameObject* pObj0 = FindGameObject(pBody0);
	//GameObject* pObj1 = FindGameObject(pBody1);
	//
	//if (!pObj0 || !pObj1) return;

	//pObj0->SetColor(btVector3(0.0, 0.0, 0.0));
	//pObj1->SetColor(btVector3(0.0, 0.0, 0.0));
}

GameObject* App::FindGameObject(std::string name)
{
	for (GameObjects::iterator iter = m_objects.begin(); iter != m_objects.end(); ++iter)
	{
		if ((*iter)->GetName() == name)
			return *iter;
	}
	return NULL;
}

void App::DrawBox(const btVector3 &halfSize)
{
	//glPushMatrix();
	//glMultMatrixf(transform);

	float halfWidth = halfSize.x();
	float halfHeight = halfSize.y();
	float halfDepth = halfSize.z();

	//glColor3f(color.x(), color.y(), color.z());

	btVector3 vertices[8] = {
	btVector3(halfWidth,halfHeight,halfDepth),
	btVector3(-halfWidth,halfHeight,halfDepth),
	btVector3(halfWidth,-halfHeight,halfDepth),
	btVector3(-halfWidth,-halfHeight,halfDepth),
	btVector3(halfWidth,halfHeight,-halfDepth),
	btVector3(-halfWidth,halfHeight,-halfDepth),
	btVector3(halfWidth,-halfHeight,-halfDepth),
	btVector3(-halfWidth,-halfHeight,-halfDepth) };

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

	glBegin(GL_TRIANGLES);

	for (int i = 0; i < 36; i += 3)
	{
		const btVector3& vert1 = vertices[indices[i]];
		const btVector3& vert2 = vertices[indices[i + 1]];
		const btVector3& vert3 = vertices[indices[i + 2]];

		btVector3 normal = (vert3 - vert1).cross(vert2 - vert1);
		normal.normalize();

		glNormal3f(normal.x(), normal.y(), normal.z());

		glVertex3f(vert1.x(), vert1.y(), vert1.z());
		glVertex3f(vert2.x(), vert2.y(), vert2.z());
		glVertex3f(vert3.x(), vert3.y(), vert3.z());
	}
	glEnd();
	//glPopMatrix();
}

void App::DrawSphere(const btScalar& radius)
{
	static int lateralSegments = 25;
	static int longitudinalSegments = 25;

	for (int i = 0; i <= lateralSegments; i++)
	{
		btScalar lat0 = SIMD_PI * (-btScalar(0.5f) + (btScalar) (i - 1) / lateralSegments);
		btScalar z0 = radius * sin(lat0);
		btScalar zr0 = radius * cos(lat0);

		btScalar lat1 = SIMD_PI * (-btScalar(0.5f) + (btScalar) i / lateralSegments);
		btScalar z1 = radius * sin(lat1);
		btScalar zr1 = radius * cos(lat1);

		glBegin(GL_QUAD_STRIP);
		for (int j = 0; j <= longitudinalSegments; j++)
		{
			btScalar lng = 2 * SIMD_PI * (btScalar)(j - 1) / longitudinalSegments;
			btScalar x = cos(lng);
			btScalar y = sin(lng);

			btVector3 normal = btVector3(x * zr0, y * zr0, z0);
			normal.normalize();
			glNormal3f(normal.x(), normal.y(), normal.z());
			glVertex3f(x * zr0, y * zr0, z0);

			normal = btVector3(x * zr1, y * zr1, z1);
			normal.normalize();
			glNormal3f(normal.x(), normal.y(), normal.z());
			glVertex3f(x * zr1, y * zr1, z1);
		}
		glEnd();
	}
}

GameObject* App::CreateGameObject(btCollisionShape* pShape, const float& mass, const btVector3& color, const btVector3& initialPotation, const btQuaternion& initialRotation)
{
	GameObject* pObject = new GameObject(pShape, mass, color, initialPotation, initialRotation);
	m_objects.push_back(pObject);
	if (m_pWorld)
	{
		m_pWorld->addRigidBody(pObject->GetRigidBody());
	}
	return pObject;
}

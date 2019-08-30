//////////////////////////////////////////////////////////////////////////
//                    _   _   ____   ____                    
//   Project         | | | | / ___) / ___)                   
//                   | | | || | __ | |                       
//                   | |_| || |_\ || \___                    
//                    \____/ \____| \ ___)     6.1           
//                                                           
//!  \file UGScriptCompiler.h
//!  \brief 三维材质编译器类
//!  \details 负责材质的解析
//!  \author sunyl
//!  \attention 
//!   Copyright (c) 1996-2012 SuperMap Software Co., Ltd. <br> 
//!   All Rights Reserved.
//!  \version 6.1
//////////////////////////////////////////////////////////////////////////

#ifndef __SCRIPTCOMPILER_H__
#define __SCRIPTCOMPILER_H__

#if _MSC_VER > 1000
#pragma once
#endif

#include "Base3D/UGPrerequisites3D.h"
#include "Base3D/UGSharedPtr.h"
#include <vector>
#include <list>

namespace UGC
{

	enum ConcreteNodeType
	{
		CNT_VARIABLE,
		CNT_VARIABLE_ASSIGN,
		CNT_WORD,
		CNT_IMPORT,
		CNT_QUOTE,
		CNT_LBRACE,
		CNT_RBRACE,
		CNT_COLON
	};

	/** The ConcreteNode is the struct that holds an un-conditioned sub-tree of parsed input */
	struct ConcreteNode;
	typedef UGSharedPtr<ConcreteNode> ConcreteNodePtr;
	typedef std::list<ConcreteNodePtr> ConcreteNodeList;
	typedef UGSharedPtr<ConcreteNodeList> ConcreteNodeListPtr;
	struct ConcreteNode
	{
		UGString token, file;
		UGuint line;
		ConcreteNodeType type;
		ConcreteNodeList children;
		ConcreteNode *parent;
	};

	/** This enum holds the types of the possible abstract nodes */
	enum AbstractNodeType
	{
		ANT_UNKNOWN,
		ANT_ATOM,
		ANT_OBJECT,
		ANT_PROPERTY,
		ANT_IMPORT,
		ANT_VARIABLE_SET,
		ANT_VARIABLE_ACCESS
	};
	class AbstractNode;
	typedef UGSharedPtr<AbstractNode> AbstractNodePtr;
	typedef std::list<AbstractNodePtr> AbstractNodeList;
	typedef UGSharedPtr<AbstractNodeList> AbstractNodeListPtr;

	class BASE3D_API AbstractNode
	{
	public:
		UGString file;
		UGuint line;
		AbstractNodeType type;
		AbstractNode *parent;
		void* context; // A holder for translation context data
	public:
		AbstractNode(AbstractNode *ptr);
		virtual ~AbstractNode(){}
		/// Returns a new AbstractNode which is a replica of this one.
		virtual AbstractNode *clone() const = 0;
		/// Returns a string value depending on the type of the AbstractNode.
		virtual UGString getValue() const = 0;
	};

	/** This is an abstract node which cannot be broken down further */
	class BASE3D_API AtomAbstractNode : public AbstractNode
	{
	public:
		UGString value;
		UGuint id;
	public:
		AtomAbstractNode(AbstractNode *ptr);
		AbstractNode *clone() const;
		UGString getValue() const;
	private:
		void parseNumber() const;
	};

	/** This specific abstract node represents a script object */
	class BASE3D_API ObjectAbstractNode : public AbstractNode
	{
	private:
		std::map<UGString,UGString> mEnv;
	public:
		UGString name, cls;
		std::vector<UGString> bases;
		UGuint id;
		UGbool abstract;
		AbstractNodeList children;
		AbstractNodeList values;
		AbstractNodeList overrides; // For use when processing object inheritance and overriding
	public:
		ObjectAbstractNode(AbstractNode *ptr);
		AbstractNode *clone() const;
		UGString getValue() const;

		void addVariable(const UGString &name);
		void setVariable(const UGString &name, const UGString &value);
		std::pair<UGbool,UGString> getVariable(const UGString &name) const;
		const std::map<UGString,UGString> &getVariables() const;
	};

	/** This abstract node represents a script property */
	class BASE3D_API PropertyAbstractNode : public AbstractNode
	{
	public:
		UGString name;
		UGuint id;
		AbstractNodeList values;
	public:
		PropertyAbstractNode(AbstractNode *ptr);
		AbstractNode *clone() const;
		UGString getValue() const;
	};

	/** This abstract node represents an import statement */
	class BASE3D_API ImportAbstractNode : public AbstractNode
	{
	public:
		UGString target, source;
	public:
		ImportAbstractNode();
		AbstractNode *clone() const;
		UGString getValue() const;
	};

	/** This abstract node represents a variable assignment */
	class BASE3D_API VariableAccessAbstractNode : public AbstractNode
	{
	public:
		UGString name;
	public:
		VariableAccessAbstractNode(AbstractNode *ptr);
		AbstractNode *clone() const;
		UGString getValue() const;
	};

	

//! \brief 三维材质类。
class BASE3D_API UGScriptCompiler
{
public:
	// The container for errors
	struct ScriptError 
	{
		UGString file, message;
		UGint line;
		UGuint code;
	};
	typedef UGSharedPtr<ScriptError> ErrorPtr;
	typedef std::list<ErrorPtr> ErrorList;

	// These are the built-in error codes
	enum{
		CE_STRINGEXPECTED,
		CE_NUMBEREXPECTED,
		CE_FEWERPARAMETERSEXPECTED,
		CE_VARIABLEEXPECTED,
		CE_UNDEFINEDVARIABLE,
		CE_OBJECTNAMEEXPECTED,
		CE_OBJECTALLOCATIONERROR,
		CE_INVALIDPARAMETERS,
		CE_DUPLICATEOVERRIDE,
		CE_UNEXPECTEDTOKEN,
		CE_OBJECTBASENOTFOUND,
		CE_UNSUPPORTEDBYRENDERSYSTEM,
		CE_REFERENCETOANONEXISTINGOBJECT
	};

	typedef std::map<UGString,UGuint> IdMap;
public:
	//! \brief 构造函数。
	UGScriptCompiler();
	//! \brief 构造函数。
	~UGScriptCompiler(){};

	UGbool Compile(const UGString &str, const UGString &source, const UGString &group);
	/// Compiles resources from the given concrete node list
	UGbool Compile(const ConcreteNodeListPtr &nodes, const UGString &group);

	/// Adds the given error to the compiler's list of errors
	void addError(UGuint code, const UGString &file, UGint line, const UGString &msg = _U(""));

	/// Returns the resource group currently set for this compiler
	const UGString &getResourceGroup() const
	{
		return m_strGroupName;
	}


private:
	AbstractNodeListPtr convertToAST(const ConcreteNodeListPtr &nodes);
	/// This built-in function processes import nodes
	void processImports(AbstractNodeListPtr &nodes);
	/// Loads the requested script and converts it to an AST
	AbstractNodeListPtr loadImportPath(const UGString &name);
	/// Returns the abstract nodes from the given tree which represent the target
	AbstractNodeListPtr locateTarget(AbstractNodeList *nodes, const UGString &target);
	/// Handles object inheritance and variable expansion
	void processObjects(AbstractNodeList *nodes, const AbstractNodeListPtr &top);
	/// Handles processing the variables
	void processVariables(AbstractNodeList *nodes);
	/// This function overlays the given object on the destination object following inheritance rules
	void overlayObject(const AbstractNodePtr &source, ObjectAbstractNode *dest);
	/// Returns true if the given class is name excluded
	UGbool isNameExcluded(const UGString &cls, AbstractNode *parent);
	/// This function sets up the initial values in word id map
	void initWordMap();

	
private:
	// Error list
	ErrorList m_Errors;

	//当前解析的脚本数据的资源组的名字
	UGString m_strGroupName;

	// This is an environment map
	typedef std::map<UGString,UGString> Environment;
	Environment mEnv;
	typedef std::map<UGString,AbstractNodeListPtr> ImportCacheMap;
	ImportCacheMap mImports; // The set of imported scripts to avoid circular dependencies
	typedef std::multimap<UGString,UGString> ImportRequestMap;
	ImportRequestMap mImportRequests; // This holds the target objects for each script to be imported

	// This stores the imports of the scripts, so they are separated and can be treated specially
	AbstractNodeList mImportTable;

	// The word -> id conversion table
	IdMap mIds;

private: // Internal helper classes and processors
	class AbstractTreeBuilder
	{
	private:
		AbstractNodeListPtr mNodes;
		AbstractNode *mCurrent;
		UGScriptCompiler *mCompiler;
	public:
		AbstractTreeBuilder(UGScriptCompiler *compiler);
		const AbstractNodeListPtr &getResult() const;
		void visit(ConcreteNode *node);
		static void visit(AbstractTreeBuilder *visitor, const ConcreteNodeList &nodes);
	};
	friend class AbstractTreeBuilder;
public:
	// This enum are built-in word id values
	enum
	{
		ID_ON = 1,
		ID_OFF = 2,
		ID_TRUE = 1,
		ID_FALSE = 2,
		ID_YES = 1,
		ID_NO = 2
	};
} ;

/// This enum defines the integer ids for keywords this compiler handles
enum
{
	ID_MATERIAL = 3,
	ID_VERTEX_PROGRAM,
	ID_GEOMETRY_PROGRAM,
	ID_FRAGMENT_PROGRAM,
	ID_TECHNIQUE,
	ID_PASS,
	ID_TEXTURE_UNIT,
	ID_VERTEX_PROGRAM_REF,
	ID_GEOMETRY_PROGRAM_REF,
	ID_FRAGMENT_PROGRAM_REF,
	ID_SHADOW_CASTER_VERTEX_PROGRAM_REF,
	ID_SHADOW_RECEIVER_VERTEX_PROGRAM_REF,
	ID_SHADOW_RECEIVER_FRAGMENT_PROGRAM_REF,
	ID_SHADOW_CASTER_MATERIAL,
	ID_SHADOW_RECEIVER_MATERIAL,

	ID_LOD_VALUES,
	ID_LOD_STRATEGY,
	ID_LOD_DISTANCES,
	ID_RECEIVE_SHADOWS,
	ID_TRANSPARENCY_CASTS_SHADOWS,
	ID_SET_TEXTURE_ALIAS,

	ID_SOURCE,
	ID_SYNTAX,
	ID_DEFAULT_PARAMS,
	ID_PARAM_INDEXED,
	ID_PARAM_NAMED,
	ID_PARAM_INDEXED_AUTO,
	ID_PARAM_NAMED_AUTO,

	ID_SCHEME,
	ID_LOD_INDEX,
	ID_GPU_VENDOR_RULE,
	ID_GPU_DEVICE_RULE,
	ID_INCLUDE, 
	ID_EXCLUDE, 

	ID_AMBIENT,
	ID_DIFFUSE,
	ID_SPECULAR,
	ID_EMISSIVE,
	ID_VERTEXCOLOUR,
	ID_SCENE_BLEND,
	ID_COLOUR_BLEND,
	ID_ONE,
	ID_ZERO,
	ID_DEST_COLOUR,
	ID_SRC_COLOUR,
	ID_ONE_MINUS_DEST_COLOUR,
	ID_ONE_MINUS_SRC_COLOUR,
	ID_DEST_ALPHA,
	ID_SRC_ALPHA,
	ID_ONE_MINUS_DEST_ALPHA,
	ID_ONE_MINUS_SRC_ALPHA,
	ID_SEPARATE_SCENE_BLEND,
	ID_SCENE_BLEND_OP,
	ID_REVERSE_SUBTRACT,
	ID_MIN,
	ID_MAX,
	ID_SEPARATE_SCENE_BLEND_OP,
	ID_DEPTH_CHECK,
	ID_DEPTH_WRITE,
	ID_DEPTH_FUNC,
	ID_DEPTH_BIAS,
	ID_ITERATION_DEPTH_BIAS,
	ID_ALWAYS_FAIL,
	ID_ALWAYS_PASS,
	ID_LESS_EQUAL,
	ID_LESS,
	ID_EQUAL,
	ID_NOT_EQUAL,
	ID_GREATER_EQUAL,
	ID_GREATER,
	ID_ALPHA_REJECTION,
	ID_ALPHA_TO_COVERAGE,
	ID_LIGHT_SCISSOR,
	ID_LIGHT_CLIP_PLANES,
	ID_TRANSPARENT_SORTING,
	ID_ILLUMINATION_STAGE,
	ID_DECAL,
	ID_CULL_HARDWARE,
	ID_CLOCKWISE,
	ID_ANTICLOCKWISE,
	ID_CULL_SOFTWARE,
	ID_BACK,
	ID_FRONT,
	ID_NORMALISE_NORMALS,
	ID_LIGHTING,
	ID_SHADING,
	ID_FLAT, 
	ID_GOURAUD,
	ID_PHONG,
	ID_POLYGON_MODE,
	ID_SOLID,
	ID_WIREFRAME,
	ID_POINTS,
	ID_POLYGON_MODE_OVERRIDEABLE,
	ID_FOG_OVERRIDE,
	ID_NONE,
	ID_LINEAR,
	ID_EXP,
	ID_EXP2,
	ID_COLOUR_WRITE,
	ID_MAX_LIGHTS,
	ID_START_LIGHT,
	ID_ITERATION,
	ID_ONCE,
	ID_ONCE_PER_LIGHT,
	ID_PER_LIGHT,
	ID_PER_N_LIGHTS,
	ID_POINT,
	ID_SPOT,
	ID_DIRECTIONAL,
	ID_POINT_SIZE,
	ID_POINT_SPRITES,
	ID_POINT_SIZE_ATTENUATION,
	ID_POINT_SIZE_MIN,
	ID_POINT_SIZE_MAX,

	ID_TEXTURE_ALIAS,
	ID_TEXTURE,
	ID_1D,
	ID_2D,
	ID_3D,
	ID_CUBIC,
	ID_UNLIMITED,
	ID_ALPHA,
	ID_GAMMA,
	ID_ANIM_TEXTURE,
	ID_CUBIC_TEXTURE,
	ID_SEPARATE_UV,
	ID_COMBINED_UVW,
	ID_TEX_COORD_SET,
	ID_TEX_ADDRESS_MODE,
	ID_WRAP,
	ID_CLAMP,
	ID_BORDER,
	ID_MIRROR,
	ID_TEX_BORDER_COLOUR,
	ID_FILTERING,
	ID_BILINEAR,
	ID_TRILINEAR,
	ID_ANISOTROPIC,
	ID_MAX_ANISOTROPY,
	ID_MIPMAP_BIAS,
	ID_COLOUR_OP,
	ID_REPLACE,
	ID_ADD,
	ID_MODULATE,
	ID_ALPHA_BLEND,
	ID_COLOUR_OP_EX,
	ID_SOURCE1,
	ID_SOURCE2,
	ID_MODULATE_X2,
	ID_MODULATE_X4,
	ID_ADD_SIGNED,
	ID_ADD_SMOOTH,
	ID_SUBTRACT,
	ID_BLEND_DIFFUSE_COLOUR,
	ID_BLEND_DIFFUSE_ALPHA,
	ID_BLEND_TEXTURE_ALPHA,
	ID_BLEND_CURRENT_ALPHA,
	ID_BLEND_MANUAL,
	ID_DOT_PRODUCT,
	ID_SRC_CURRENT,
	ID_SRC_TEXTURE,
	ID_SRC_DIFFUSE,
	ID_SRC_SPECULAR,
	ID_SRC_MANUAL,
	ID_COLOUR_OP_MULTIPASS_FALLBACK,
	ID_ALPHA_OP_EX,
	ID_ENV_MAP,
	ID_SPHERICAL,
	ID_PLANAR,
	ID_CUBIC_REFLECTION,
	ID_CUBIC_NORMAL,
	ID_SCROLL,
	ID_SCROLL_ANIM,
	ID_ROTATE,
	ID_ROTATE_ANIM,
	ID_SCALE,
	ID_WAVE_XFORM,
	ID_SCROLL_X,
	ID_SCROLL_Y,
	ID_SCALE_X,
	ID_SCALE_Y,
	ID_SINE,
	ID_TRIANGLE,
	ID_SQUARE,
	ID_SAWTOOTH,
	ID_INVERSE_SAWTOOTH,
	ID_TRANSFORM,
	ID_BINDING_TYPE,
	ID_VERTEX,
	ID_FRAGMENT,
	ID_CONTENT_TYPE,
	ID_NAMED,
	ID_SHADOW,
	ID_TEXTURE_SOURCE,
	ID_SHARED_PARAMS,
	ID_SHARED_PARAM_NAMED,
	ID_SHARED_PARAMS_REF,

	ID_PARTICLE_SYSTEM,
	ID_EMITTER,
	ID_AFFECTOR,

	ID_COMPOSITOR,
	ID_TARGET,
	ID_TARGET_OUTPUT,

	ID_INPUT,
	ID_PREVIOUS,
	ID_TARGET_WIDTH,
	ID_TARGET_HEIGHT,
	ID_TARGET_WIDTH_SCALED,
	ID_TARGET_HEIGHT_SCALED,
	ID_COMPOSITOR_LOGIC,
	ID_TEXTURE_REF,
	ID_SCOPE_LOCAL,
	ID_SCOPE_CHAIN,
	ID_SCOPE_GLOBAL,
	ID_POOLED,
	//ID_GAMMA, - already registered for material
	ID_NO_FSAA,
	ID_ONLY_INITIAL,
	ID_VISIBILITY_MASK,
	ID_LOD_BIAS,
	ID_MATERIAL_SCHEME,
	ID_SHADOWS_ENABLED,

	ID_CLEAR,
	ID_STENCIL,
	ID_RENDER_SCENE,
	ID_RENDER_QUAD,
	ID_IDENTIFIER,
	ID_FIRST_RENDER_QUEUE,
	ID_LAST_RENDER_QUEUE,
	ID_QUAD_NORMALS,
	ID_CAMERA_FAR_CORNERS_VIEW_SPACE,
	ID_CAMERA_FAR_CORNERS_WORLD_SPACE,

	ID_BUFFERS,
	ID_COLOUR,
	ID_DEPTH,
	ID_COLOUR_VALUE,
	ID_DEPTH_VALUE,
	ID_STENCIL_VALUE,

	ID_CHECK,
	ID_COMP_FUNC,
	ID_REF_VALUE,
	ID_MASK,
	ID_FAIL_OP,
	ID_KEEP,
	ID_INCREMENT,
	ID_DECREMENT,
	ID_INCREMENT_WRAP,
	ID_DECREMENT_WRAP,
	ID_INVERT,
	ID_DEPTH_FAIL_OP,
	ID_PASS_OP,
	ID_TWO_SIDED,
	ID_END_BUILTIN_IDS
};

}

#endif 



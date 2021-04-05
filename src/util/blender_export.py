import bpy
import struct
from mathutils import Matrix, Vector, Color

EXPORT_GLOBAL_MATRIX = Matrix()
MatGL = Matrix(([-1, 0, 0, 0],
                [ 0, 0, 1, 0],
                [ 0, 1, 0, 0],
                [ 0, 0, 0, 1]))
InvMatGL = MatGL.inverted()

def writeTransform(file, obj):
    m = MatGL * obj.matrix_local * InvMatGL
    m.transpose()
    for row in m:
        for value in row:
            file.write(struct.pack('f', value))

def mesh_triangulate(me):
    import bmesh
    bm = bmesh.new()
    bm.from_mesh(me)
    bmesh.ops.triangulate(bm, faces=bm.faces)
    bm.to_mesh(me)
    bm.free()

# get all meshes
def writeMesh(file, obj):
    obs = [(obj, obj.matrix_world)]

    for ob, ob_mat in obs:
        try:
            me = ob.original.to_mesh()
        except RuntimeError:
            me = None

        if me is None:
            continue

        mesh_triangulate(me)
        #me.transform(EXPORT_GLOBAL_MATRIX @ ob_mat)
        me_verts = me.vertices[:]
        if ob_mat.determinant() < 0.0:
            me.flip_normals()

        # Vert
        #for v in me_verts:
        #    print(v.co)

        me.calc_loop_triangles()

        dict = {}
        indices = []

        for tri in me.loop_triangles:
            for vert_index in tri.vertices:
                vertex = me.vertices[vert_index]
                vc = MatGL @ vertex.co

                vp = (vc.x, vc.y, vc.z)

                index = dict.setdefault(vp, len(dict))

                indices.append(index)

        #bpy.data.meshes.remove(me)

        vertices = [None] * len(dict)
        for key in dict.keys():
            vertices[dict.get(key)] = key

        file.write(struct.pack('i', len(indices)))
        for i in indices:
            file.write(struct.pack('i', i))

        file.write(struct.pack('i', len(vertices)))
        for vp in vertices:
            for v in vp:
                file.write(struct.pack('f', v))

def collect(path):
    print('convert scene')
    file = open(path, 'wb')

    for i, obj in enumerate(bpy.context.scene.objects):
        print(obj.name)
        type = -1
        if obj.type == 'MESH':
            type = 0
        if obj.type == 'LAMP':
            type = 1
        if type == -1:
            continue

        file.write(struct.pack('i', type))
        writeTransform(file, obj)
        if type == 0:
            writeMesh(file, obj)

    file.close()
    print('done')

collect('C:/Users/int11/Desktop/dev/exporter/mesh.lvl')

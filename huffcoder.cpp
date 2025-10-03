#include "..\..\G_LIBS\cpp\types.h"

#include <stdio.h>
#include <stdlib.h>

// This constant can be avoided by explicitly
// calculating height of Huffman Tree
#define MAX_TREE_HT 100

#define MAX_TREE_ENTRIES 16

struct tree_entry_s
{
	u16 bits;
	byte n_bits;
	u16 mag_len;
};

typedef struct tree_entry_s tree_entry_t;

struct tree_s
{
	byte num_entries;
	tree_entry_t entries[MAX_TREE_ENTRIES];
};

typedef struct tree_s tree_t;

// 113505344
// 118909069
// 220495166
// 388868392
// 549294028
// 530820904
// 385181213
// 271271165
// 202782797
// 78628165
// 24604708
// 12357655
//#include "tree.h"

tree_t static_tree =
{
	12,

	{	{ 0x0000, 2, 4 },
		{ 0x0002, 3, 7 },
		{ 0x0007, 3, 5 },
		{ 0x0004, 3, 6 },
		{ 0x0005, 3, 3 },
		{ 0x0006, 4, 1 },
		{ 0x000C, 4, 2 },
		{ 0x0007, 4, 8 },
		{ 0x001A, 5, 0 },
		{ 0x0037, 6, 9 },
		{ 0x006C, 7, 11 },
		{ 0x006D, 7, 10 }, }
};

static inline byte calc_magnitude(u16 val)
{
	byte mag = 0;
	while (val != 0 || mag > 16) {
		val >>= 1;
		mag++;
	}
	return mag;
}

struct bit_buffer_s {
	byte* buf;
	u16 buf_capacity;
	u32 n_bits;
	u32 current_bit;
};

typedef struct bit_buffer_s bit_buffer_t;

// typedef struct codec_s codec_t;

typedef struct codec_s
{
	bit_buffer_t bb;
	tree_t *tree;
} codec_t;

bool codec_init(codec_t *codec, tree_t *tree);
u16 encode(codec_t *codec, i16 *wave, u16 wave_len, byte *buf, u16 max_num_bytes);
u16 decode(codec_t *codec, byte *buf, u16 num_bits, i16 *wave, u16 max_wave_len);
void huff_gen_tree(char data[], unsigned int freq[], int size, tree_t *tree);



/// @brief Расчитывает количество байт, необходимое для хранения n_bits количсетва бит
/// @param n_bits Количество бит
/// @return Количество байт
static inline u32 num_bytes(u32 n_bits) { return (n_bits + 7) >> 3; }

//***************************************************************************
//*                               PROTOTYPES
//***************************************************************************

bool bb_init(bit_buffer_t* bb, byte* buf, u16 buf_capacity, u32 n_bits);
bool bb_test(bit_buffer_t* bb, u16 bits, byte n_bits);
u16 bb_get_bits(bit_buffer_t* bb, byte n_bits);
bool bb_put_bits(bit_buffer_t* bb, u16 bits, byte n_bits);
u16 bb_buf_size(bit_buffer_t* bb);


// Encodes a signed 16-bit integer using Zigzag encoding
static inline u16 zigzag_encode16(i16 n)
{
	return (n << 1) ^ (n >> 15);
}

// Decodes an unsigned 16-bit integer back to a signed 16-bit integer
static inline i16 zigzag_decode16(u16 n)
{
	return (n >> 1) ^ -(i16)(n & 1);
}

// Encodes a signed 32-bit integer using Zigzag encoding
static inline u32 zigzag_encode32(i32 n)
{
	// If n is positive, it maps to 2 * n
	// If n is negative, it maps to (2 * |n|) - 1
	// This can be achieved with (n << 1) ^ (n >> 31)
	// The right shift by 31 acts as a sign extension for negative numbers,
	// resulting in all 1s (0xFFFFFFFF) for negative n, and all 0s for positive n.
	return (n << 1) ^ (n >> 31);
}

// Decodes an unsigned 32-bit integer back to a signed 32-bit integer
static inline i32 zigzag_decode32(u32 n)
{
	// If the least significant bit is 0, it's a positive number: n / 2
	// If the least significant bit is 1, it's a negative number: -(n + 1) / 2
	// This can be achieved with (n >> 1) ^ -(n & 1)
	return (n >> 1) ^ -(i32)(n & 1);
}

// Encodes a signed 64-bit integer using Zigzag encoding
static inline u64 zigzag_encode64(i64 n)
{
	return (n << 1) ^ (n >> 63);
}

// Decodes an unsigned 64-bit integer back to a signed 64-bit integer
static inline i64 zigzag_decode64(u64 n)
{
	return (n >> 1) ^ -(i64)(n & 1);
}

#define return_false_if_true(x) { if (x) return false; }



static const u16 masks[16] = {0x1, 0x3, 0x7, 0xf, 0x1f, 0x3f, 0x7f, 0xff,
0x1ff, 0x3ff, 0x7ff, 0xfff, 0x1fff, 0x3fff, 0x7fff, 0xffff};



static u16 get_n_bits(byte *buf, u32 current_bit, byte n_bits)
{
	u32 index = current_bit >> 3;
	byte bit_no = current_bit & 0x7;
	u16 result = buf[index];
	i8 shift = 8 - (bit_no + n_bits);

	if (shift >= 0) {
		result >>= shift;
	} else {
		result <<= 8;
		result |= buf[index+1];
		result >>= shift + 8;
	}

	return result & masks[n_bits-1];
}


bool bb_init(bit_buffer_t* bb, byte* buf, u16 buf_capacity, u32 n_bits)
{
	return_false_if_true(bb == NULL);
	return_false_if_true(buf_capacity == 0);
	return_false_if_true(num_bytes(n_bits) > buf_capacity);

	bb->buf = buf;
	bb->buf_capacity = buf_capacity;
	bb->n_bits = n_bits;
	bb->current_bit = 0;
	return true;
}

bool bb_test(bit_buffer_t* bb, u16 bits, byte n_bits)
{
	return_false_if_true(bb == NULL);
	return_false_if_true(n_bits == 0 || n_bits > 16);
	return_false_if_true(bb->current_bit + n_bits > bb->n_bits);

	return get_n_bits(bb->buf, bb->current_bit, n_bits) == bits;
}

u16 bb_get_bits(bit_buffer_t* bb, byte n_bits)
{
	return_false_if_true(bb == NULL);
	return_false_if_true(n_bits == 0 || n_bits > 16);
	return_false_if_true(bb->current_bit + n_bits > bb->n_bits);
	u16 result = get_n_bits(bb->buf, bb->current_bit, n_bits);
	bb->current_bit += n_bits;
	return result;
}

bool bb_put_bits(bit_buffer_t* bb, u16 bits, byte n_bits)
{
	return_false_if_true(bb == NULL);
	return_false_if_true(n_bits == 0 || n_bits > 16);
	return_false_if_true(num_bytes(bb->current_bit + n_bits) > bb->buf_capacity);

	u32 index = bb->current_bit >> 3;
	byte bit_no = bb->current_bit & 0x7;
	u16 result = bb->buf[index] & ~masks[7-bit_no];

	if (bit_no + n_bits <= 8)
	{
		result |= (bits & masks[n_bits-1]) << (8 - n_bits - bit_no);
		bb->buf[index] = result;
	}
	else
	{
		result <<= 8;
		result |= (bits & masks[n_bits-1]) << (16 - n_bits - bit_no);
		bb->buf[index] = result >> 8;
		bb->buf[index+1] = result;
	}

	bb->n_bits += n_bits;
	bb->current_bit += n_bits;

	return true;
}

u16 bb_buf_size(bit_buffer_t* bb)
{
	return_false_if_true(bb == NULL);

	return num_bytes(bb->n_bits);
}


// A Huffman tree node
struct MinHeapNode {

	// One of the input characters
	byte data;

	// Frequency of the character
	u32 freq;

	// Left and right child of this node
	struct MinHeapNode *left, *right;
};

// A Min Heap: Collection of
// min-heap (or Huffman tree) nodes
struct MinHeap
{
	// Current size of min heap
	u32 size;

	// capacity of min heap
	u32 capacity;

	// Array of minheap node pointers
	struct MinHeapNode** array;
};

// A utility function allocate a new
// min heap node with given character
// and frequency of the character
static struct MinHeapNode* newNode(char data, unsigned freq)
{
	struct MinHeapNode* temp = (struct MinHeapNode*)malloc(sizeof(struct MinHeapNode));

	temp->left = temp->right = NULL;
	temp->data = data;
	temp->freq = freq;

	return temp;
}

// A utility function to create
// a min heap of given capacity
static struct MinHeap* createMinHeap(unsigned capacity)
{
	struct MinHeap* minHeap = (struct MinHeap*)malloc(sizeof(struct MinHeap));

	// current size is 0
	minHeap->size = 0;

	minHeap->capacity = capacity;

	minHeap->array = (struct MinHeapNode**)malloc(minHeap->capacity * sizeof(struct MinHeapNode*));
	return minHeap;
}

// A utility function to
// swap two min heap nodes
static void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b)
{
	struct MinHeapNode* t = *a;
	*a = *b;
	*b = t;
}

// The standard minHeapify function.
static void minHeapify(struct MinHeap* minHeap, u32 idx)
{
	u32 smallest = idx;
	u32 left = 2 * idx + 1;
	u32 right = 2 * idx + 2;

	if (left < minHeap->size
		&& minHeap->array[left]->freq
		< minHeap->array[smallest]->freq)
		smallest = left;

	if (right < minHeap->size
		&& minHeap->array[right]->freq
		< minHeap->array[smallest]->freq)
		smallest = right;

	if (smallest != idx) {
		swapMinHeapNode(&minHeap->array[smallest],
			&minHeap->array[idx]);
		minHeapify(minHeap, smallest);
	}
}

// A utility function to check
// if size of heap is 1 or not
static inline int isSizeOne(struct MinHeap* minHeap)
{
	return (minHeap->size == 1);
}

// A standard function to extract
// minimum value node from heap
static struct MinHeapNode* extractMin(struct MinHeap* minHeap)

{

	struct MinHeapNode* temp = minHeap->array[0];
	minHeap->array[0] = minHeap->array[minHeap->size - 1];

	--minHeap->size;
	minHeapify(minHeap, 0);

	return temp;
}

// A utility function to insert
// a new node to Min Heap
static void insertMinHeap(struct MinHeap* minHeap,
	struct MinHeapNode* minHeapNode)

{

	++minHeap->size;
	int i = minHeap->size - 1;

	while (i
		&& minHeapNode->freq
		< minHeap->array[(i - 1) / 2]->freq) {

		minHeap->array[i] = minHeap->array[(i - 1) / 2];
		i = (i - 1) / 2;
	}

	minHeap->array[i] = minHeapNode;
}

// A standard function to build min heap
void buildMinHeap(struct MinHeap* minHeap)

{

	int n = minHeap->size - 1;
	int i;

	for (i = (n - 1) / 2; i >= 0; --i)
		minHeapify(minHeap, i);
}

// Utility function to check if this node is leaf
static inline int isLeaf(struct MinHeapNode* root)
{
	return !(root->left) && !(root->right);
}

// Creates a min heap of capacity
// equal to size and inserts all character of
// data[] in min heap. Initially size of
// min heap is equal to capacity
static struct MinHeap* createAndBuildMinHeap(char data[],
	unsigned int freq[], int size)

{

	struct MinHeap* minHeap = createMinHeap(size);

	for (int i = 0; i < size; ++i)
		minHeap->array[i] = newNode(data[i], freq[i]);

	minHeap->size = size;
	buildMinHeap(minHeap);

	return minHeap;
}

// The main function that builds Huffman tree
static struct MinHeapNode* buildHuffmanTree(char data[],
	unsigned int freq[], int size)

{
	struct MinHeapNode *left, *right, *top;

	// Step 1: Create a min heap of capacity
	// equal to size. Initially, there are
	// modes equal to size.
	struct MinHeap* minHeap
		= createAndBuildMinHeap(data, freq, size);

	// Iterate while size of heap doesn't become 1
	while (!isSizeOne(minHeap)) {

		// Step 2: Extract the two minimum
		// freq items from min heap
		left = extractMin(minHeap);
		right = extractMin(minHeap);

		// Step 3: Create a new internal
		// node with frequency equal to the
		// sum of the two nodes frequencies.
		// Make the two extracted node as
		// left and right children of this new node.
		// Add this node to the min heap
		// '$' is a special value for internal nodes, not
		// used
		top = newNode('$', left->freq + right->freq);

		top->left = left;
		top->right = right;

		insertMinHeap(minHeap, top);
	}

	// Step 4: The remaining node is the
	// root node and the tree is complete.
	return extractMin(minHeap);
}

static void gen_tree(struct MinHeapNode* root, int top, u16 bits, tree_t *tree)
{
	// Assign 0 to left edge and recur
	if (root->left) {
		gen_tree(root->left, top + 1, bits << 1, tree);
	}

	// Assign 1 to right edge and recur
	if (root->right) {
		gen_tree(root->right, top + 1, (bits << 1) | 1, tree);
	}

	// If this is a leaf node, then
	// it contains one of the input
	// characters, print the character
	// and its code from arr[]
	if (isLeaf(root)) {
		if (tree->num_entries > MAX_TREE_ENTRIES) {
			tree->num_entries++;
			return;      
		}
		tree_entry_t *te = &tree->entries[tree->num_entries++];
		te->bits = bits;
		te->mag_len = root->data;
		te->n_bits = top;
	}
}

static int comp (tree_entry_t *e1, tree_entry_t *e2)
{
	if (e1->n_bits > e2->n_bits)
	{
		return 1;
	}
	else if (e1->n_bits < e2->n_bits)
	{
		return -1;
	};

	return 0;
}

void huff_gen_tree(char data[], unsigned int freq[], int size, tree_t *tree)
{
	struct MinHeapNode* root = buildHuffmanTree(data, freq, size);
	tree->num_entries = 0;
	gen_tree(root, 0, 0, tree);
	if (tree->num_entries > MAX_TREE_ENTRIES)
		return;
	qsort((void *)tree->entries, tree->num_entries, 
		sizeof(tree_entry_t), (int(*) (const void *, const void *)) comp);
}

bool codec_init(codec_t *codec, tree_t *tree)
{
	codec->tree = tree;
	return true;
}

static byte get_mag_len(codec_t *codec)
{
	bit_buffer_t *bb = &codec->bb;
	byte num_entries = codec->tree->num_entries;
	tree_entry_t *te = codec->tree->entries;

	while (num_entries--)
	{
		if (bb_test(bb, te->bits, te->n_bits))
		{
			return te->mag_len;
		};
	};

	return 0;
}

u32 huffencode(i16 *wave, u16 wave_len, byte *buf, u16 max_num_bytes)
{
	i16 *pwave = wave;
	u16 len = wave_len;
	i16 sample;
	byte mag;

	codec_t codec;

	codec.bb.buf = buf;
	codec.bb.buf_capacity = max_num_bytes;
	codec.bb.n_bits = 0;
	codec.bb.current_bit = 0;
	codec.tree = &static_tree;

	while (len--)
	{
		sample = zigzag_encode16(*pwave++);
		mag = calc_magnitude(sample);
		tree_entry_t *te = &codec.tree->entries[mag];
		bb_put_bits(&codec.bb, te->bits, te->n_bits);
		bb_put_bits(&codec.bb, sample, mag);
	};

	return codec.bb.n_bits;
}

u16 huffdecode(byte *buf, u32 num_bits, i16 *wave, u16 max_wave_len)
{
	byte n_bits;
	i16 *pwave = wave;

	codec_t codec;

	codec.bb.buf = buf;
	codec.bb.buf_capacity = num_bytes((u32) num_bits);
	codec.bb.n_bits = num_bits;
	codec.bb.current_bit = 0; 
	codec.tree = &static_tree;

	while (!(n_bits = get_mag_len(&codec)))
	{
		*pwave++ = zigzag_decode16(bb_get_bits(&codec.bb, n_bits));
	};

	return true;
}


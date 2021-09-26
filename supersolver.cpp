#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef unsigned long uint32_t;
typedef unsigned __int64 uint64_t;

/////// Solver ////////////////////////////////////////////////////////////////

struct PieceInfo
{
   uint32_t busyType;
   uint32_t coverage[5];
   char     signature;
   int      skipToNext;
};

PieceInfo pieceInfo[] =
{
// F
   { 0x001, 0x18, 0x30, 0x10, 0x00, 0x00, 'F', 7 },
   { 0x001, 0x08, 0x0e, 0x04, 0x00, 0x00, 'F', 6 },
   { 0x001, 0x08, 0x0c, 0x18, 0x00, 0x00, 'F', 5 },
   { 0x001, 0x08, 0x1c, 0x04, 0x00, 0x00, 'F', 4 },
   { 0x001, 0x18, 0x0c, 0x08, 0x00, 0x00, 'F', 3 },
   { 0x001, 0x08, 0x38, 0x10, 0x00, 0x00, 'F', 2 },
   { 0x001, 0x08, 0x18, 0x0c, 0x00, 0x00, 'F', 1 },
   { 0x001, 0x08, 0x1c, 0x10, 0x00, 0x00, 'F', 0 },
// I
   { 0x002, 0xf8, 0x00, 0x00, 0x00, 0x00, 'I', 1 },
   { 0x002, 0x08, 0x08, 0x08, 0x08, 0x08, 'I', 0 },
// L
   { 0x004, 0x78, 0x40, 0x00, 0x00, 0x00, 'L', 7 },
   { 0x004, 0x78, 0x08, 0x00, 0x00, 0x00, 'L', 6 },
   { 0x004, 0x08, 0x08, 0x08, 0x18, 0x00, 'L', 5 },
   { 0x004, 0x08, 0x08, 0x08, 0x0c, 0x00, 'L', 4 },
   { 0x004, 0x08, 0x78, 0x00, 0x00, 0x00, 'L', 3 },
   { 0x004, 0x08, 0x0f, 0x00, 0x00, 0x00, 'L', 2 },
   { 0x004, 0x18, 0x10, 0x10, 0x10, 0x00, 'L', 1 },
   { 0x004, 0x18, 0x08, 0x08, 0x08, 0x00, 'L', 0 },
// P
   { 0x008, 0x18, 0x18, 0x08, 0x00, 0x00, 'P', 7 },
   { 0x008, 0x18, 0x18, 0x10, 0x00, 0x00, 'P', 6 },
   { 0x008, 0x08, 0x18, 0x18, 0x00, 0x00, 'P', 5 },
   { 0x008, 0x08, 0x0c, 0x0c, 0x00, 0x00, 'P', 4 },
   { 0x008, 0x38, 0x18, 0x00, 0x00, 0x00, 'P', 3 },
   { 0x008, 0x38, 0x30, 0x00, 0x00, 0x00, 'P', 2 },
   { 0x008, 0x18, 0x1c, 0x00, 0x00, 0x00, 'P', 1 },
   { 0x008, 0x18, 0x38, 0x00, 0x00, 0x00, 'P', 0 },
// N
   { 0x010, 0x18, 0x70, 0x00, 0x00, 0x00, 'N', 7 },
   { 0x010, 0x18, 0x0e, 0x00, 0x00, 0x00, 'N', 6 },
   { 0x010, 0x38, 0x60, 0x00, 0x00, 0x00, 'N', 5 },
   { 0x010, 0x38, 0x0c, 0x00, 0x00, 0x00, 'N', 4 },
   { 0x010, 0x08, 0x18, 0x10, 0x10, 0x00, 'N', 3 },
   { 0x010, 0x08, 0x0c, 0x04, 0x04, 0x00, 'N', 2 },
   { 0x010, 0x08, 0x08, 0x18, 0x10, 0x00, 'N', 1 },
   { 0x010, 0x08, 0x08, 0x0c, 0x04, 0x00, 'N', 0 },
// T
   { 0x020, 0x38, 0x10, 0x10, 0x00, 0x00, 'T', 3 },
   { 0x020, 0x08, 0x08, 0x1c, 0x00, 0x00, 'T', 2 },
   { 0x020, 0x08, 0x0e, 0x08, 0x00, 0x00, 'T', 1 },
   { 0x020, 0x08, 0x38, 0x08, 0x00, 0x00, 'T', 0 },
// U
   { 0x040, 0x28, 0x38, 0x00, 0x00, 0x00, 'U', 3 },
   { 0x040, 0x38, 0x28, 0x00, 0x00, 0x00, 'U', 2 },
   { 0x040, 0x18, 0x08, 0x18, 0x00, 0x00, 'U', 1 },
   { 0x040, 0x18, 0x10, 0x18, 0x00, 0x00, 'U', 0 },
// V
   { 0x080, 0x38, 0x08, 0x08, 0x00, 0x00, 'V', 3 },
   { 0x080, 0x38, 0x20, 0x20, 0x00, 0x00, 'V', 2 },
   { 0x080, 0x08, 0x08, 0x0e, 0x00, 0x00, 'V', 1 },
   { 0x080, 0x08, 0x08, 0x38, 0x00, 0x00, 'V', 0 },
// W
   { 0x100, 0x18, 0x30, 0x20, 0x00, 0x00, 'W', 3 },
   { 0x100, 0x18, 0x0c, 0x04, 0x00, 0x00, 'W', 2 },
   { 0x100, 0x08, 0x18, 0x30, 0x00, 0x00, 'W', 1 },
   { 0x100, 0x08, 0x0c, 0x06, 0x00, 0x00, 'W', 0 },
// X
   { 0x200, 0x08, 0x1c, 0x08, 0x00, 0x00, 'X', 0 },
// Y
   { 0x400, 0x78, 0x10, 0x00, 0x00, 0x00, 'Y', 7 },
   { 0x400, 0x78, 0x20, 0x00, 0x00, 0x00, 'Y', 6 },
   { 0x400, 0x08, 0x3c, 0x00, 0x00, 0x00, 'Y', 5 },
   { 0x400, 0x08, 0x1e, 0x00, 0x00, 0x00, 'Y', 4 },
   { 0x400, 0x08, 0x18, 0x08, 0x08, 0x00, 'Y', 3 },
   { 0x400, 0x08, 0x0c, 0x08, 0x08, 0x00, 'Y', 2 },
   { 0x400, 0x08, 0x08, 0x18, 0x08, 0x00, 'Y', 1 },
   { 0x400, 0x08, 0x08, 0x0c, 0x08, 0x00, 'Y', 0 },
// Z
   { 0x800, 0x18, 0x10, 0x30, 0x00, 0x00, 'Z', 3 },
   { 0x800, 0x18, 0x08, 0x0c, 0x00, 0x00, 'Z', 2 },
   { 0x800, 0x08, 0x38, 0x20, 0x00, 0x00, 'Z', 1 },
   { 0x800, 0x08, 0x0e, 0x02, 0x00, 0x00, 'Z', 0 },
};

uint32_t puzzle[64];
uint32_t busyPieces;

inline bool place(int x, int y, int type)
{
   PieceInfo& piece = pieceInfo[type];
   if(!((puzzle[y]   & (piece.coverage[0]<<x)) ||
        (puzzle[y+1] & (piece.coverage[1]<<x)) ||
        (puzzle[y+2] & (piece.coverage[2]<<x)) ||
        (puzzle[y+3] & (piece.coverage[3]<<x)) ||
        (puzzle[y+4] & (piece.coverage[4]<<x))))
   {
      busyPieces |= piece.busyType;
      puzzle[y]   |= (piece.coverage[0]<<x);
      puzzle[y+1] |= (piece.coverage[1]<<x);
      puzzle[y+2] |= (piece.coverage[2]<<x);
      puzzle[y+3] |= (piece.coverage[3]<<x);
      puzzle[y+4] |= (piece.coverage[4]<<x);
      return true;
   }
   return false;
}

inline void remove(int x, int y, int type)
{
   PieceInfo& piece = pieceInfo[type];
   busyPieces &= ~piece.busyType;
   puzzle[y]   &= ~(piece.coverage[0]<<x);
   puzzle[y+1] &= ~(piece.coverage[1]<<x);
   puzzle[y+2] &= ~(piece.coverage[2]<<x);
   puzzle[y+3] &= ~(piece.coverage[3]<<x);
   puzzle[y+4] &= ~(piece.coverage[4]<<x);
}

struct StepInfo
{
  int ref;
  int type;
};
int currentStep;
StepInfo solution[12];

struct Coord
{
  int x, y;
};
Coord refOrder[60];
int xmin, xmax, ymin, ymax;


inline bool find_ref()
{
   int& ref = solution[currentStep].ref;
   for(ref++; ref<60; ref++)
   {
      if(!(puzzle[refOrder[ref].y] & (1<<(refOrder[ref].x+3))))
         return true;
   }
   return false;
}


bool Setup()
{
   memset(puzzle, 0xff, sizeof(uint32_t)*64);
   xmin = 32;
   ymin = 64;
   xmax = ymax = 0;
   for(int i=0; i<60; i++)
   {
      int x = refOrder[i].x;
      int y = refOrder[i].y;
      puzzle[y] &= ~(1<<(x+3)); // note that "puzzle" array is off by 3 squares horizontally
      if(x<xmin) xmin = x;
      if(x>xmax) xmax = x;
      if(y<ymin) ymin = y;
      if(y>ymax) ymax = y;
   }

   busyPieces = 0;

   currentStep = 0;
   solution[currentStep].ref = -1;
   solution[currentStep].type = -1;
   return find_ref();
}

bool TryNext()
{
   if(currentStep == 12)
      return false;

   int currentRef = solution[currentStep].ref;
   int x = refOrder[currentRef].x;
   int y = refOrder[currentRef].y;
   int& type = solution[currentStep].type;
   if(type >= 0)
      remove(x, y, type);

   for(type++; type<63; type++)
   {
      if(!(busyPieces & pieceInfo[type].busyType))
      {
         if(place(x, y, type))
         {
            if(++currentStep<12)
            {
               solution[currentStep].ref = currentRef;
               find_ref();
               solution[currentStep].type = -1;
            }
            return true;
         }
      }
      else
         type += pieceInfo[type].skipToNext;
   }
   return false;
}

bool Solve()
{
   do
   {
      if(!TryNext())
      {
         if(currentStep == 0)
            return false;
         currentStep--;
      }
   }
   while(currentStep != 12);

   return true;
}

/////// Solver-based output routines //////////////////////////////////////////

bool Show(FILE* fout)
{
   if(currentStep == 12)
   {
      int stride = (xmax-xmin+1);
      int vsize = stride*(ymax-ymin+1);
      char* visual = new char[vsize];
      memset(visual, ' ', vsize);
      for(int item=0; item<12; item++)
      {
         int x = refOrder[solution[item].ref].x;
         int y = refOrder[solution[item].ref].y;
         int type = solution[item].type;
         for(int row=0; row<5; row++)
            for(int col=0; col<8; col++)
               if(pieceInfo[type].coverage[row] & (1<<col))
                  visual[(y+row-ymin)*stride+x+col-3-xmin] = pieceInfo[type].signature;
      }
      for(int line=0; line<ymax-ymin+1; line++)
      {
         fwrite(visual+line*stride, stride, 1, fout);
         fputc('\n', fout);
      }
      fputc('\n', fout);
      fflush(fout);
      delete[] visual;
      return true;
   }
   return false;
}

/////// Generator /////////////////////////////////////////////////////////////

typedef unsigned short index_type;
typedef signed char coord_type;

#define COORD_X(v) (*((coord_type*)&(v)))
#define COORD_Y(v) (*((coord_type*)&(v)+1))
#define STEP_LEFT(v)  (COORD_X(v)--)
#define STEP_RIGHT(v) (COORD_X(v)++)
#define STEP_UP(v)    (COORD_Y(v)--)
#define STEP_DOWN(v)  (COORD_Y(v)++)
#define BELOW(v1, v2) (COORD_Y(v1)<COORD_Y(v2)||(COORD_Y(v1)==COORD_Y(v2)&&COORD_X(v1)<=COORD_X(v2)))

struct Item
{
   static index_type used_cells[32];
   static index_type growth_cells[64]; // note that this can be static!
   int used_count;
   int growth_count;

   void step_normalize(index_type seed);
};

index_type Item::used_cells[32];
index_type Item::growth_cells[64];

void Item::step_normalize(index_type seed)
{
   growth_cells[growth_count+0] =
   growth_cells[growth_count+1] =
   growth_cells[growth_count+2] =
   growth_cells[growth_count+3] = seed;
   STEP_LEFT(growth_cells[growth_count+0]);
   STEP_RIGHT(growth_cells[growth_count+1]);
   STEP_UP(growth_cells[growth_count+2]);
   STEP_DOWN(growth_cells[growth_count+3]);
   growth_count += 4;

   // New growth cells must be:
   // a) above the root;
   // b) not in used_cells array already (can't be the last element, though)
   // c) not in growth_cells array already
   
   int prev_growth_count = growth_count-4;

   for(int i=prev_growth_count; i<growth_count; i++)
   {
      index_type test_cell = growth_cells[i];

      // a)
      if(BELOW(test_cell, used_cells[0])) // root check
      {
         growth_cells[i--] = growth_cells[--growth_count];
         continue;
      }

      // b)
      bool bCollide = false;
      for(int j=1; j<used_count-1; j++)
         if(test_cell == used_cells[j])
         {
            bCollide = true;
            break;
         }

      // c)
      if(!bCollide)
         for(int j=0; j<prev_growth_count; j++)
            if(test_cell == growth_cells[j])
            {
               bCollide = true;
               break;
            }

      // remove colliding cell (note i-- to retry at the same location)
      if(bCollide)
      {
         growth_cells[i--] = growth_cells[--growth_count];
         continue;
      }
   }
}

typedef void (*use_item_type)(Item&);

void dive(Item& base_item, int index, int stop_level, use_item_type f_use_item) 
{
   Item item;
   item.used_count = base_item.used_count;
   item.growth_count = base_item.growth_count;

   index_type growth_cell = item.growth_cells[index];
   item.used_cells[item.used_count++] = growth_cell;
   if(item.used_count == stop_level)
   {
      f_use_item(item);
      return;
   }
   item.step_normalize(growth_cell);

   for(int i=index+1; i<item.growth_count; i++)
      dive(item, i, stop_level, f_use_item);
}

void start(int stop_level, use_item_type f_use_item)
{
   Item item;
   index_type root = 0;
   item.used_cells[0] = root;
   item.used_count = 1;
   item.growth_count = 0;
   item.step_normalize(root);

   for(int i=0; i<item.growth_count; i++)
      dive(item, i, stop_level, f_use_item);
}

////////// Check for "canonical" pieces ///////////////////////////////////////

class Catcher
{
public:
   void reset(int syms, int width);
   void record(int sym, coord_type x, coord_type y);
   bool canonical();

private:
   unsigned int store[8][4]; // 128 bits should be enough for n=20
   int mSyms;
   int mWidth;
};

void Catcher::reset(int syms, int width)
{
   mSyms = syms;
   mWidth = width;
   memset(store, 0, sizeof(unsigned int)*mSyms*4);
}

inline void Catcher::record(int sym, coord_type x, coord_type y)
{
   int bit = int(x) + int(y)*mWidth;
   store[sym][bit>>5] |= 1<<(bit&31);
}

bool Catcher::canonical()
{
   for(int j=1; j<mSyms; j++)
   {
      for(int i=0; i<4; i++)
      {
         if(store[0][i] < store[j][i])
            break;
         if(store[0][i] > store[j][i])
            return false;
      }
   }
   return true;
}

Catcher catcher;


bool is_canonical(Item& item)
{
// This can be optimized further, potential 2x speedup
   coord_type maxx = -100, maxy = -100;
   coord_type minx = 100, miny = 100;

   for(int i=0; i<item.used_count; i++)
   {
      coord_type x = COORD_X(item.used_cells[i]);
      coord_type y = COORD_Y(item.used_cells[i]);
      if(x < minx) minx = x;
      if(y < miny) miny = y;
      if(x > maxx) maxx = x;
      if(y > maxy) maxy = y;
   }

   int xw = maxx-minx;
   int yw = maxy-miny;

   if(yw < xw)
      return false;

   if(yw == xw) // need to test 8 symmetries
   {
      catcher.reset(8, xw+1);
      for(int i=0; i<item.used_count; i++)
      {
         coord_type x = COORD_X(item.used_cells[i])-minx;
         coord_type y = COORD_Y(item.used_cells[i])-miny;

         catcher.record(0, x, y);
         catcher.record(1, xw-x, y);
         catcher.record(2, x, yw-y);
         catcher.record(3, xw-x, yw-y);
         catcher.record(4, y, x);
         catcher.record(5, y, xw-x);
         catcher.record(6, yw-y, x);
         catcher.record(7, yw-y, xw-x);
      }
   }
   else // need to test only 4 symmetries
   {
      catcher.reset(4, xw+1);
      for(int i=0; i<item.used_count; i++)
      {
         coord_type x = COORD_X(item.used_cells[i])-minx;
         coord_type y = COORD_Y(item.used_cells[i])-miny;

         catcher.record(0, x, y);
         catcher.record(1, xw-x, y);
         catcher.record(2, x, yw-y);
         catcher.record(3, xw-x, yw-y);
      }
   }
   return catcher.canonical();
}


////////// Generator to Solver binding ////////////////////////////////////////

// 8x8 rotator
uint64_t calc_sig(uint64_t configuration)
{
   uint64_t best = 0xffffffffffffffffLL;
   uint64_t test = configuration;
   while(!(test&0xff)) test>>=8;
   while(!(test&0x0101010101010101LL)) test>>=1;
   if(test < best)
      best = test;
   test=_byteswap_uint64(configuration);
   while(!(test&0xff)) test>>=8;
   while(!(test&0x0101010101010101LL)) test>>=1;
   if(test < best)
      best = test;
   test = ((test&0xf0f0f0f0f0f0f0f0LL)>>4)|((test&0x0f0f0f0f0f0f0f0fLL)<<4);
   test = ((test&0xccccccccccccccccLL)>>2)|((test&0x3333333333333333LL)<<2);
   test = ((test&0xaaaaaaaaaaaaaaaaLL)>>1)|((test&0x5555555555555555LL)<<1);
   while(!(test&0xff)) test>>=8;
   while(!(test&0x0101010101010101LL)) test>>=1;
   if(test < best)
      best = test;
   test=_byteswap_uint64(test);
   while(!(test&0xff)) test>>=8;
   while(!(test&0x0101010101010101LL)) test>>=1;
   if(test < best)
      best = test;

   test = 0LL;
   uint64_t mask = 1LL;
   for(int low=0; low<8; low++)
      for(int high=0; high<8; high++, mask<<=1)
         if(configuration & (1LL<<(high*8+low)))
            test |= mask;

   while(!(test&0xff)) test>>=8;
   while(!(test&0x0101010101010101LL)) test>>=1;
   if(test < best)
      best = test;
   test=_byteswap_uint64(test);
   while(!(test&0xff)) test>>=8;
   while(!(test&0x0101010101010101LL)) test>>=1;
   if(test < best)
      best = test;
   test = ((test&0xf0f0f0f0f0f0f0f0LL)>>4)|((test&0x0f0f0f0f0f0f0f0fLL)<<4);
   test = ((test&0xccccccccccccccccLL)>>2)|((test&0x3333333333333333LL)<<2);
   test = ((test&0xaaaaaaaaaaaaaaaaLL)>>1)|((test&0x5555555555555555LL)<<1);
   while(!(test&0xff)) test>>=8;
   while(!(test&0x0101010101010101LL)) test>>=1;
   if(test < best)
      best = test;
   test=_byteswap_uint64(test);
   while(!(test&0xff)) test>>=8;
   while(!(test&0x0101010101010101LL)) test>>=1;
   if(test < best)
      best = test;

   return best;
}

FILE* output;
FILE* fsigs;
int solutions;
int attempts;
int cycles;

int _cdecl reforder_sort(const void* left, const void* right)
{
   Coord& cleft = *(Coord*)left;
   Coord& cright = *(Coord*)right;

   if(cleft.y<cright.y)
      return -1;
   if(cleft.y>cright.y)
      return 1;
   if(cleft.x<cright.x)
      return -1;
   if(cleft.x>cright.x)
      return 1;
   return 0;
}

void try_puzzle(Item& item)
{
   if(!is_canonical(item)) // remove unneeded rotations
      return;

   if(++attempts == 10000)
   {
      cycles ++;
      attempts = 0;
      printf("Now trying #%d0000\n", cycles);
   }

   int i;

   // Find bounds
   // TODO: is_canonical already has those
   int minx = 256, miny=256, maxx = -256, maxy = -256;
   for(i=0; i<item.used_count; i++)
   {
      int x = int(COORD_X(item.used_cells[i]));
      int y = int(COORD_Y(item.used_cells[i]));
      if(x < minx) minx = x;
      if(x > maxx) maxx = x;
      if(y < miny) miny = y;
      if(y > maxy) maxy = y;
   }

   // For canonical pieces we have maxy-miny >= maxx-minx guaranteed

   // Create solver reference order
   for(i=0; i<20; i++)
   {
      int x = int(COORD_X(item.used_cells[i]));
      int y = int(COORD_Y(item.used_cells[i]));
      refOrder[i].x = x-minx;
      refOrder[i].y = y-miny;
   }

   qsort(refOrder, 20, sizeof(refOrder[0]), reforder_sort);

   for(i=0; i<20; i++)
   {
      int x = refOrder[i].x;
      int y = refOrder[i].y;
      refOrder[i+20].x = x+maxx-minx+2;
      refOrder[i+20].y = y;
      if(maxx-minx < 8)
      {
         refOrder[i+40].x = x+(maxx-minx+2)*2;
         refOrder[i+40].y = y;
      }
      else
      {
         refOrder[i+40].x = x;
         refOrder[i+40].y = y+(maxy-miny+2);
      }
   }

   Setup();
   if(Solve())
   {
      solutions++;
      if(maxy-miny < 8)
      {
         // Create "signature"
         uint64_t sig = 0LL;
         for(i=0; i<20; i++)
         {
            int x = int(COORD_X(item.used_cells[i]));
            int y = int(COORD_Y(item.used_cells[i]));
            sig |= (1LL<<(8*(y-miny)+(x-minx)));
         }
         sig = calc_sig(sig);

         fprintf(output, "%d [%08x%08x]\n\n", solutions, uint32_t(sig>>32), uint32_t(sig));
         fprintf(fsigs, "%08x%08x\n", uint32_t(sig>>32), uint32_t(sig));
      }
      else
      {
         fprintf(output, "%d [****************]\n\n", solutions);
      }

      Show(output);
   }
}


////////// Startup ////////////////////////////////////////////////////////////

void main()
{
   output = fopen("solutions.txt", "rt");
   if(output) 
   {
      fclose(output);
      printf("Cannot overwrite solutions file\n");
      return;
   }
   output = fopen("solutions.txt", "wt");
   fsigs = fopen("updatedsigs.txt", "wt");

   solutions = 0;
   attempts = 0;
   cycles = 0;

   start(20, try_puzzle);

   fprintf(output, "Done. Tried %d%04d possible configurations.\n", cycles, attempts);

   fclose(output);
   fclose(fsigs);
}

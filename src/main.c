#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


/**
 * writes string to the file
 */
size_t fileWrite(const char *str, FILE *file)
{
  size_t size = strlen(str);
  return fwrite(str, sizeof(char), size, file);
}

/**
 * converts int to string
 */
char *toString(int n)
{
  char *str = malloc(256);
  sprintf(str, "%d", n);
  return str;
}

int main(int argc, char *argv[])
{
  //checking args
  if (argc <= 1)
  {
    fprintf(stderr, "Usage:\n\t%s\t<bmp_file> [-o <output_file>]\n", argv[0]);
    return 1;
  }

  //check if file exists file
  FILE *file = NULL;
  file = fopen(argv[1], "rb");
  if (!file)
  {
    fprintf(stderr, "Cannot open file '%s'!\n", argv[1]);
    return 2;
  }
  fclose(file);
  
  //read bmp data
  unsigned char *data;
  int width, height, components;
  data = stbi_load(argv[1], &width, &height, &components, 0); 

  //create new file
  char *name;
  int output = 0;

  if (argc > 2)
  {
    //check for filename in parameter
    int i;
    for (i = 0; i < argc; ++i)
    {
      if (strcmp(argv[i], "-o") == 0)
      {
        output = 1; 
        name = argv[i+1];
        break;
      }
    }
  } 

  char *s = malloc((size_t)(width*height*components*5+16*3) );
  memset(s, 0, (size_t)(width*height*components*5+16*3) );
  strcat(s, "int imageWidth = ");
  strcat(s, toString(width) );
  strcat(s, ";\n");
  strcat(s, "int imageHeight = ");
  strcat(s, toString(height) );
  strcat(s, ";\n");
  strcat(s, "int imageComponents = ");
  strcat(s, toString(components) );
  strcat(s, ";\n");
  strcat(s, "const unsigned char imageData[] = { \n");
  int i;
  for(i = 0; i < width*height*components; i++)
  {
    strcat(s, toString((int)data[i]) );
    if (i != (width*height*components)-1) strcat(s, ", ");
    if (i%12 == 0 && i != 0) strcat(s, "\n");
  }
  strcat(s, " };\n");

  if (!output)
  {
    printf("%s\n", s);
  }
  else
  {
    file = fopen(name, "wb");
    fprintf(file, s);
    fclose(file);
  }


}

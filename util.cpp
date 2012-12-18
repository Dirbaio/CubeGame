#include "util.h"

static map<string, Texture*> imgMap;

Texture* loadTexture(string path) 
{
        map<string, Texture*>::iterator it;
        it = imgMap.find(path);
        if (it == imgMap.end()) {
                cerr << "* Loading image: " << path << endl;
                Texture* t = new Texture();
                t->loadFromFile(path);
                t->setSmooth(false);

				imgMap[path] = t;
				return t;
        }

        return it->second;
}

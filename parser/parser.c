#include "configure.h"


static const char def_config[] =  
    "# количество шариков\nMAX_COUNT = %d\n\n"
    "# радиус шариков\nRADIUS = %.2f\n\n"
    "# вывод энергии [0/1]\nCHECK_ENERGY = %d\n\n"
    "# границы [0/1]\nBORDERS = %d\n\n# тип внешнего взаимодействия:\n"
    "# 0 - нет\n# 1 - центральное\n# 2 - гравитационное поле\n"
    "EXT_INTERACTION = %d\n\n# тип взаимодействия:\n"
    "# 0 - нет\n# 1 - гравитация [между шариками]\n"
    "# 2 - электромагнитное взаимодействие [пока только Кулон]\n"
    "# 3 - 1 + 2\nINTERACTION = %d\n\n"
    "# размерность пространства для Ньютоновской механики GRAVITY_DIM = 3\n"
    "GRAVITY_DIM = %d\n\n# гравитационная постоянная\nG = %.2f\n\n"
    "# электрическая постоянная\nE = %.2f\n\n# тип соудорения:\n"
    "# 0 - упруго\n# 1 - неупруго\n# 2 - слипание [жидкость]\n"
    "HIT_TYPE = %d\n\n# скорость при стартовом разлёте\nSTART_SPEED = %d";


int get_parameter(FILE *f, p_format *p, char *a)
{
    int c;
    int i = 0;
    int state = S_TEXT;
    char label[64];

    while ((c = fgetc(f)) != EOF)
    {
        if (c == T_END && state == S_DATA)
        {
            a[i] = T_NULL;
            i = 0;
            for (i = 0; p[i].name != NULL; i++)
                if (!strcmp(label, p[i].name))
                    return p[i].number;
        }
        else if (c == T_DATA && state == S_TEXT)
        {
            label[i-1] = T_NULL;
            i = 0;
        }

        if (c == T_COMMENT)
            state = S_COMMENT;
        else if (c == T_DATA && state == S_TEXT)
            state = S_DATA;
        else if (c == T_END && state == S_COMMENT)
            state = S_TEXT;
        else if (c == T_END)
            state = S_TEXT;

        if (state == S_DATA && c != T_DATA && c != T_SPACE)
        {
            a[i] = c;
            i++;
        }
        else if (state == S_TEXT && c != T_END)
        {
            label[i] = c;
            i++;
        }
    }

    if (c == EOF && state == S_DATA)
    {
        a[i] = T_NULL;
        i = 0;
        for (i = 0; p[i].name != NULL; i++)
            if (!strcmp(label, p[i].name))
                return p[i].number;
    }
    
    return -1;
}


rc configure(char *file)
{
    rc conf = {100, 1.0f, 1, 0, 2, 3, 10.0f, 10.0f, 0, 10, 1};
    p_format params[] = {
		{"MAX_COUNT", 0},
		{"RADIUS", 1},
		{"CHECK_ENERGY", 2},
		{"BORDERS", 3},
		{"EXT_INTERACTION", 4},
		{"INTERACTION", 5},
		{"GRAVITY_DIM", 6},
		{"G", 7},
		{"E", 8},
		{"HIT_TYPE", 9},
		{"START_SPEED", 10},
		{0, 0}
	};
	FILE *f;
	int opt;
	char arg[64];
	
	if ((f = fopen(file, "r")))
	{
		do {
			opt = get_parameter(f, params, arg);

			switch (opt) {
				case 0: conf.max_count = atoi(arg); break;
				case 1: conf.radius = (float) atof(arg); break;
				case 2: conf.check_energy = atoi(arg); break;
				case 3: conf.borders = atoi(arg); break;
				case 4: conf.ext_interaction = atoi(arg); break;
				case 5: conf.interaction = atoi(arg); break;
				case 6: conf.gravity_dim = atoi(arg); break;
				case 7: conf.G = (float) atof(arg); break;
				case 8: conf.E = (float) atof(arg); break;
				case 9: conf.hit_type = atoi(arg); break;
				case 10: conf.start_speed = atoi(arg); break;
			}
		} while (opt != -1);
		
		fclose(f);
	}
	else {
		if ((f = fopen(file, "wb")) != NULL) {
    		fprintf(f, def_config, conf.max_count, conf.radius, conf.check_energy, 
                conf.borders, conf.ext_interaction, conf.interaction,
                conf.gravity_dim, conf.G, conf.E, conf.hit_type, conf.start_speed); 
    		fclose(f);
        }
    	else 
    		printf("Ошибка создания файла конфигурации!\n");
	}
	return conf;
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


typedef enum
{
    Rescue,
    Escort,
    Outlaw
} MissionType;


typedef struct
{
    MissionType type;
    char ground[64];
    char client[64];
    char escort[64];
    char name[64];
    char zone[64];
    int segment;
    int floor;
} MissionInfo;


// SV.test_grounds.Missions["OutlawQuest"] = { Complete = COMMON.MISSION_INCOMPLETE, Type = COMMON.MISSION_TYPE_OUTLAW, DestZone = "debug_zone", DestSegment = 4, DestFloor = 9, TargetSpecies = "riolu" }
//SV.test_grounds.Missions["CaterQuest"] = { Complete = COMMON.MISSION_INCOMPLETE, Type = COMMON.MISSION_TYPE_RESCUE, DestZone = "debug_zone", DestSegment = 4, DestFloor = 4, TargetSpecies = "caterpie" }

static void append(char *text, int maxSize, char *toAppend) {
  size_t offset = strlen(text);
  snprintf(&(text[offset]), maxSize, "%s", toAppend);
}

static void GenQuest(MissionInfo *mission)
{
    FILE *fp;
    char buffer[512];
    char temp[128];

    // put in the name
    // This is now hardcoded
    //snprintf(buffer, 512, "SV.%s.Missions[\"%s\"] ", mission->ground, mission->name);
    snprintf(buffer, 512, "SV.%s.Missions[\"%s\"] ", "missions", mission->name);

    if (mission->type == 0)
    {
        snprintf(temp, 128, "= { Complete = COMMON.MISSION_INCOMPLETE, Type = COMMON.MISSION_TYPE_RESCUE, ");
    }
    else if (mission->type == 1)
    {
        snprintf(temp, 128, "= { Complete = COMMON.MISSION_INCOMPLETE, Type = COMMON.MISSION_TYPE_ESCORT, ");
    }
    else
    {
        snprintf(temp, 128, "= { Complete = COMMON.MISSION_INCOMPLETE, Type = COMMON.MISSION_TYPE_OUTLAW, ");
    }

    append(buffer, 128, temp);

    snprintf(temp, 128, "DestZone = \"%s\", ", mission->zone);
    append(buffer, 128, temp);
    snprintf(temp, 128, "DestSegment = %d, ", mission->segment);
    append(buffer, 128, temp);
    snprintf(temp, 128, "DestFloor = %d, ", mission->floor);
    append(buffer, 128, temp);


    if (mission->type == Escort)
    {
        snprintf(temp, 128, "TargetSpecies = \"%s\", ", mission->client);
        append(buffer, 128, temp);

        snprintf(temp, 128, "EscortSpecies = \"%s\" }", mission->escort);
        append(buffer, 128, temp);
    }
    else
    {
        snprintf(temp, 128, "TargetSpecies = \"%s\" }", mission->client);
        append(buffer, 128, temp);
    }

    fp = fopen("output.txt","w");
    fwrite(buffer, 1, strlen(buffer), fp);
    fclose(fp);
}


int main()
{

    MissionInfo *mission = malloc(sizeof(MissionInfo));
    memset(mission, 0, sizeof(MissionInfo));

    printf("Welcome to Rev's PMDO Quest generator\n");
    printf("This tool will ask you questions about what your quest will be and will generate some quest data for you.\n");

    //printf("What is the ground map name?\n");
    //scanf("%s", mission.ground);
    printf("What type of quest? (0 = Rescue, 1 = Escort, 2 = Outlaw)\n");
    scanf("%d", &mission->type);
    printf("What is the quest name?\n");
    scanf("%s", mission->name);
    printf("What is the dungeon name for this quest? (zone_name)\n");
    scanf("%s", mission->zone);
    printf("What is the dungeon segment for this quest? (default = 0)\n");
    scanf("%d", &mission->segment);
    printf("What floor does the quest take place at? (default = 1)\n");
    scanf("%d", &mission->floor);
    printf("Who is the target? (name of pokemon in lowercase)\n");
    scanf("%s", mission->client);

    if (mission->type == Escort)
    {
        printf("Who is the escort? (name of pokemon in lowercase)\n");
        scanf("%s", mission->escort);
    }

    GenQuest(mission);
    free(mission);

    return 0;
}

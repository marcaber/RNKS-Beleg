#pragma once

extern void processMsg(struct genericMsg *pubPtr);
extern void fillAnnnouncement(struct MCAnnouncement *req, char *topic, char *mcAddr);
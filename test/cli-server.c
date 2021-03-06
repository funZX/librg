#define LIBRG_IMPLEMENTATION
#define LIBRG_DEBUG
#include <librg.h>

int batid;
int footime;
librg_peer *peer;

void on_connect_request(librg_event *event) {
    u32 secret = librg_data_ru32(event->data);

    if (secret != 42) {
        librg_event_reject(event);
    }
}

void on_connect_accepted(librg_event *event) {
    librg_log("on_connect_accepted\n");
    librg_entity *blob = event->entity;

    // blob->position.x = (float)(2000 - rand() % 4000);
    // blob->position.y = (float)(2000 - rand() % 4000);
    blob->position.x = 0;
    blob->position.y = 0;

    librg_log("spawning player %u at: %f %f %f\n",
        event->entity->id,
        blob->position.x,
        blob->position.y,
        blob->position.z
    );

    footime = 4;
    peer = event->peer;

    enet_peer_timeout(event->peer, 10, 2000, 3000);
}

void on_connect_refused(librg_event *event) {
    librg_log("on_connect_refused\n");
}

void on_connect_disconnect(librg_event *event) {
    librg_log("entity %d peer disconnected: %x\n", event->entity->id, event->peer);
}

void on_entity_create(librg_event *event) {

}

void on_entity_update(librg_event *event) {
    librg_log("update ent %d for client %x\n", event->entity->id, event->peer);
}

void on_entity_remove(librg_event *event) {
    librg_log("remove ent %d for client %x\n", event->entity->id, event->peer);
}

void measure(void *userptr) {
    librg_ctx *ctx = (librg_ctx *)userptr;

    if (!ctx || !ctx->network.host) return;

    static u32 lastdl = 0;
    static u32 lastup = 0;

    f32 dl = (ctx->network.host->totalReceivedData - lastdl) * 8.0f / ( 1000.0f * 1000 ) ; // mbps
    f32 up = (ctx->network.host->totalSentData     - lastup) * 8.0f / ( 1000.0f * 1000 ) ; // mbps

    lastdl = ctx->network.host->totalReceivedData;
    lastup = ctx->network.host->totalSentData;

    // librg_dbg("librg_update: took %f ms. Current used bandwidth D/U: (%f / %f) mbps. \r", ctx->last_update, dl, up);

    if (peer && --footime < 1) {
        librg_dbg("\npeer %x becoming streamer of the entity 2\n", peer);
        librg_entity_control_set(ctx, batid, peer);
        peer = NULL;
        return;
    }

    if (peer && --footime < 2) {
        librg_dbg("\npeer %x becoming streamer of the entity\n", peer);
        librg_entity_control_set(ctx, batid, peer);
        return;
    }
}

void on_update(librg_event *e) {
    if (e->entity->id == batid) {
        // zpl_printf("update for our bat came\n");
    }
}

int main() {
    char *test = "===============      SERVER      =================\n" \
                 "==                                              ==\n" \
                 "==                 ¯\\_(ツ)_/¯                   ==\n" \
                 "==                                              ==\n" \
                 "==================================================\n";
    librg_log("%s\n\n", test);

    librg_option_set(LIBRG_MAX_ENTITIES_PER_BRANCH, 4);

    librg_ctx ctx     = {0};

    ctx.tick_delay      = 1000;
    ctx.mode            = LIBRG_MODE_SERVER;
    ctx.world_size      = zpl_vec3f(5000.0f, 5000.0f, 0.f);
    ctx.min_branch_size = zpl_vec3f(-1, -1, -1);
    ctx.max_entities    = 60000;
    ctx.max_connections = 1200;

    librg_init(&ctx);

    librg_event_add(&ctx, LIBRG_CONNECTION_REQUEST, on_connect_request);
    librg_event_add(&ctx, LIBRG_CONNECTION_ACCEPT, on_connect_accepted);
    librg_event_add(&ctx, LIBRG_CONNECTION_REFUSE, on_connect_refused);
    librg_event_add(&ctx, LIBRG_CONNECTION_DISCONNECT, on_connect_disconnect);
    librg_event_add(&ctx, LIBRG_CLIENT_STREAMER_UPDATE, on_update);

    librg_event_add(&ctx, LIBRG_ENTITY_CREATE, on_entity_create);
    librg_event_add(&ctx, LIBRG_ENTITY_UPDATE, on_entity_update);
    librg_event_add(&ctx, LIBRG_ENTITY_REMOVE, on_entity_remove);

    librg_network_start(&ctx, (librg_address) { .port = 7779 });

    // for (isize i = 0; i < 1; i++) {
    //     librg_entity *enemy = librg_entity_create(&ctx, 0);

    //     //librg_attach_foo(&ctx, enemy, NULL);
    //     // enemy->position.x = (float)(2000 - rand() % 4000);
    //     // enemy->position.x = (float)(2000 - rand() % 4000);
    //     enemy->position.x = 10;
    //     enemy->position.y = 10;

    //     batid = enemy->id;
    // }

    zpl_timer *tick_timer = zpl_timer_add(ctx.timers);
    tick_timer->user_data = (void *)&ctx; /* provide ctx as a argument to timer */
    zpl_timer_set(tick_timer, 1.0, -1, measure);
    zpl_timer_start(tick_timer, 1.0);

    while (true) {
        librg_tick(&ctx);
        zpl_sleep_ms(1);
    }

    librg_network_stop(&ctx);
    librg_free(&ctx);

    return 0;
}

#include <stdio.h>
#include <gfx.hpp>
using namespace gfx;
using bitmap_t = bitmap<rgb_pixel<24>>;
using color_t = color<typename bitmap_t::pixel_type>;
constexpr static const size16 bmp_size(80,50);
constexpr static const size_t bmp_bytes = bitmap_t::sizeof_buffer(bmp_size);
static uint8_t bmp_data[bmp_bytes];

// prints a source as 4-bit grayscale ASCII
template <typename Source>
void print_source(const Source& src) {
    static const char *col_table = " .,-~;+=x!1%$O@#";
    using gsc4 = gfx::pixel<gfx::channel_traits<gfx::channel_name::L,4>>;
    for(int y = 0;y<src.dimensions().height;++y) {
        for(int x = 0;x<src.dimensions().width;++x) {
            typename Source::pixel_type px;
            src.point(gfx::point16(x,y),&px);
            const auto px2 = gfx::convert<typename Source::pixel_type,gsc4>(px);
            size_t i =px2.template channel<0>();
            putchar(col_table[i]);
        }
        putchar('\n');
    }
}

int main(int argc, char**argv) {
    file_stream fs("microsd-card-icon.svg",io::file_mode::read);
    if(!fs.caps().read) {
        printf("file not found.\n");
        return 1;
    }
    svg_doc doc;
    gfx_result res;
   
    res=gfx::svg_doc::read(&fs,&doc);
    if(res!=gfx_result::success) {
        printf("Error: %d\n",(int)res);
    }
    fs.close();
    bitmap_t bmp(bmp_size,bmp_data);
    bmp.fill(bmp.bounds(),color_t::white);
    draw::svg(bmp,(srect16)bmp.bounds(),doc,doc.scale(bmp.dimensions()));
    print_source(bmp);
   
    
    return 0;
}
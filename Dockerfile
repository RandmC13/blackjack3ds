FROM devkitpro/devkitarm AS build
WORKDIR /tmp/output/
ADD ./ ./

RUN make

# Copy build files to host system
FROM scratch as binaries
COPY --from=build /tmp/output/*.3dsx ./
COPY --from=build /tmp/output/*.elf ./
COPY --from=build /tmp/output/*.smdh ./

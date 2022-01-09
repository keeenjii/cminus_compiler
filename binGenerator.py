#coding: utf-8 
opcode = ('nop', 'add', 'addi', 'sub', 'subi', 'mult', 'nop', 'div',
          'lw', 'sw', 'nop', 'sra', 'sla', 'and', 'andi', 'or',
          'ori', 'xor', 'xori', 'eq', 'cmp', 'cmpi', 'not', 'j', 
          'jr', 'beq', 'bne', 'jal', 'break', 'out', 'in')

register = ('$t0', '$t1', '$t2', '$t3', '$t4', '$t5', '$t6', '$t7', '$t8',
            '$t9', '$t10', '$t11', '$t12', '$t13', '$t14', '$t15', '$t16',
            '$a0', '$a1', '$a2', '$a3', '$a4', '$a5', '$a6', '$ret', '$sp',
            '$zero', '$gp', '$RE', '$LO', '$HI', '$ra')
'''
Instrução R: Opcode(6bits), R1(5bits), R2(5bits), Deslocamento(5 bits), FUNCT(6 bits)
Instrução I: Opcode(6bits), R1(5bits), Rd(5bits), Imediato ou endereço(16bits)
Instrução J: Opcode(6bits), Endereço(16bits)
'''

##resolver problema dos labels e branchs
assemblyFile = open('./asscode.txt', 'r')
lineNumber = 2
labels = {}
for a in assemblyFile:
    lab = a.split()
    if lab:
        if lab[0] not in opcode:
            labels[lab[0]] = lineNumber
    if len(a) > 0:
      lineNumber+=1
assemblyFile.close()   
 

assemblyFile = open('./asscode.txt', 'r')
lineNumber = 2
with open('binarycode.txt', 'w') as binaryFile:
    binaryFile.write('%s\n' % "// Código binário gerado automaticamente")
    for line in assemblyFile:
        inst = line.split()
        if len(inst) == 1:
            if inst[0] in opcode: ## jr, break ou in
                binaryFile.write("%s\n" % 'mem_instrucao[{}] = {{ 6\'d{}, 26\'d0 }};{:>10}'.format(lineNumber, 
                                                                                          opcode.index(inst[0]),
                                                                                          '//{}'.format(line)))
                lineNumber+=1 
            else: #label
                binaryFile.write("%s\n" % 'mem_instrucao[{}] = {{ 32\'d0 }};{:>10}'.format(lineNumber,
                                                                                          '//{}'.format(line)))
                lineNumber+=1
        elif len(inst) == 2:
            linelabel = labels[inst[1]]
            binaryFile.write("%s\n" % 'mem_instrucao[{}] = {{ 6\'d{}, 26\'d{} }};{:>20}'.format(lineNumber,
                                                                                               opcode.index(inst[0]),
                                                                                               linelabel,
                                                                                               '//{}'.format(line)))
            lineNumber+=1
        
        elif len(inst) == 3:
            if inst[0] == 'div' or inst[0] == 'mult' or inst[0] == 'out':
                binaryFile.write("%s\n" % 'mem_instrucao[{}] = {{ 6\'d{}, 5\'d{}, 5\'d{}, 16\'d0 }};{:>10}'.format(lineNumber,
                                                                                                                  opcode.index(inst[0]),
                                                                                                                  register.index(inst[1]),
                                                                                                                  register.index(inst[2]),
                                                                                                                  '//{}'.format(line)))
                lineNumber+=1
            elif inst[0] == 'not':
                binaryFile.write("%s\n" % 'mem_instrucao[{}] = {{ 6\'d{}, 5\'d{}, 5\'d0, 5\'d{}, 11\'d0 }};{:>10}'.format(lineNumber,
                                                                                                                         opcode.index(inst[0]),
                                                                                                                         register.index(inst[2]),
                                                                                                                         register.index(inst[1]),
                                                                                                                         '//{}'.format(line)))
                lineNumber+=1
        elif len(inst) == 4:
            if inst[0][len(inst[0])-1] == 'i' or inst[0] == 'lw' or inst[0] == 'sw' or inst[0] == 'beq' or inst[0] == 'bne':
                if inst[0] == 'beq' or inst[0] == 'bne':
                    labelOffset = labels[inst[3]] - lineNumber
                    if labelOffset >= 0:
                        binaryFile.write("%s\n" % 'mem_instrucao[{}] = {{ 6\'d{}, 5\'d{}, 5\'d{}, 16\'d{} }};{:>15}'.format(lineNumber,
                                                                                                                        opcode.index(inst[0]),
                                                                                                                        register.index(inst[1]),
                                                                                                                        register.index(inst[2]),
                                                                                                                        labelOffset,
                                                                                                                        '//{}'.format(line)))
                        lineNumber+=1
                    else:
                        binaryFile.write("%s\n" % 'mem_instrucao[{}] = {{ 6\'d{}, 5\'d{}, 5\'d{}, -16\'d{} }};{:>15}'.format(lineNumber,
                                                                                                                        opcode.index(inst[0]),
                                                                                                                        register.index(inst[1]),
                                                                                                                        register.index(inst[2]),
                                                                                                                        abs(labelOffset),
                                                                                                                        '//{}'.format(line)))
                        lineNumber+=1
                else:
                    if int(inst[3]) >= 0:
                        if inst[0] != 'sw':
                            binaryFile.write("%s\n" % 'mem_instrucao[{}] = {{ 6\'d{}, 5\'d{}, 5\'d{}, 16\'d{} }};{:>10}'.format(lineNumber,
                                                                                                                            opcode.index(inst[0]),
                                                                                                                            register.index(inst[2]),
                                                                                                                            register.index(inst[1]),
                                                                                                                                int(inst[3]),
                                                                                                                                '//{}'.format(line)))
                            lineNumber+=1
                        else:
                            binaryFile.write("%s\n" % 'mem_instrucao[{}] = {{ 6\'d{}, 5\'d{}, 5\'d{}, 16\'d{} }};{:>10}'.format(lineNumber,
                                                                                                                            opcode.index(inst[0]),
                                                                                                                            register.index(inst[1]),
                                                                                                                            register.index(inst[2]),
                                                                                                                                int(inst[3]),
                                                                                                                                '//{}'.format(line)))
                            lineNumber+=1                
                    else:        
                        binaryFile.write("%s\n" % 'mem_instrucao[{}] = {{ 6\'d{}, 5\'d{}, 5\'d{}, -16\'d{} }};{:>10}'.format(lineNumber,
                                                                                                                           opcode.index(inst[0]),
                                                                                                                           register.index(inst[2]),
                                                                                                                           register.index(inst[1]),
                                                                                                                            abs(int(inst[3])),
                                                                                                                            '//{}'.format(line)))
                        lineNumber+=1 
            else:
               
                binaryFile.write("%s\n" % 'mem_instrucao[{}] = {{ 6\'d{}, 5\'d{}, 5\'d{}, 5\'d{}, 11\'d0 }};{:>10}'.format(lineNumber,
                                                                                                                          opcode.index(inst[0]),
                                                                                                                          register.index(inst[2]),
                                                                                                                          register.index(inst[3]),
                                                                                                                          register.index(inst[1]),
                                                                                                                          '//{}'.format(line)))
                lineNumber+=1
    print('Código binário gerado salvo em binarycode.txt')